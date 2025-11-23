import java.util.concurrent.TimeUnit;

public class ControlPanel {

    public enum State {
        IDLE("等待输入状态"),
        READING_INPUT("读取输入状态"),
        VERIFYING("验证密码状态"),
        LOCKED("锁定状态"),
        FUNCTION_SELECT("系统功能选择状态");

        private final String description;

        State(String description) {
            this.description = description;
        }

        public String getValue() {
            return description;
        }
    }

    private static final String PASSWORD = "1234";
    private static final int MAX_LENGTH = 4;
    private static final int MAX_ATTEMPTS = 3;
    private static final long LOCK_DURATION_MS = 120 * 1000;

    private State state;
    private StringBuilder currentInput;
    private int failureCount;
    private long lockStartTime;

    public ControlPanel() {
        this.state = State.IDLE;
        this.currentInput = new StringBuilder();
        this.failureCount = 0;
        this.lockStartTime = 0;

        System.out.println("--- SafeHome 控制面板 FSM 启动 ---");
        System.out.println("初始状态: " + this.state.getValue());
    }

    private void displayMessage(String message) {
        System.out.println("[系统] " + message);
    }

    private boolean checkLockStatus() {
        if (this.state == State.LOCKED) {
            long timePassed = System.currentTimeMillis() - this.lockStartTime;
            if (timePassed >= LOCK_DURATION_MS) {
                displayMessage("锁定时间已超过 120 秒。");
                this.state = State.IDLE;
                this.failureCount = 0;
                displayMessage("解锁成功。[转换] LOCKED -> IDLE (" + this.state.getValue() + ")");
                return true;
            } else {
                long remainingSeconds = (LOCK_DURATION_MS - timePassed) / 1000;
                displayMessage("面板仍处于锁定状态，剩余 " + remainingSeconds + " 秒。");
                return false;
            }
        }
        return true;
    }

    public void inputKey(char key) {
        if (!checkLockStatus()) {
            return;
        }

        State currentState = this.state;
        State nextState = this.state;

        if (currentState == State.IDLE) {
            this.currentInput.setLength(0);
            this.currentInput.append(key);
            nextState = State.READING_INPUT;
        }
        else if (currentState == State.READING_INPUT) {
            this.currentInput.append(key);
            int inputLen = this.currentInput.length();

            if (inputLen < MAX_LENGTH) {
            } else if (inputLen == MAX_LENGTH) {
                nextState = State.VERIFYING;
            } else {
                this.currentInput.setLength(MAX_LENGTH);
                nextState = State.VERIFYING;
            }
        }
        if (nextState == State.VERIFYING) {
            displayMessage("密码长度已达 4 位，正在验证...");
            String inputStr = this.currentInput.toString();

            if (inputStr.equals(PASSWORD)) {
                this.failureCount = 0;
                nextState = State.FUNCTION_SELECT;
                displayMessage("密码正确，进入系统功能选择。");
            } else {
                this.failureCount++;
                this.currentInput.setLength(0);

                if (this.failureCount >= MAX_ATTEMPTS) {
                    this.lockStartTime = System.currentTimeMillis();
                    nextState = State.LOCKED;
                    displayMessage("密码错误 " + this.failureCount + " 次。面板锁定 120 秒！");
                } else {
                    nextState = State.IDLE;
                    displayMessage("密码错误，请重新输入。当前失败次数: " + this.failureCount);
                }
            }
        }

        if (nextState != currentState) {
            System.out.printf("[转换] %s -> %s (%s)%n", currentState.name(), nextState.name(), nextState.getValue());
            this.state = nextState;
        } else {
            System.out.printf("当前状态: %s。接收字符 '%c'%n", this.state.getValue(), key);
        }
    }

    public static void main(String[] args) throws InterruptedException {
        ControlPanel panel = new ControlPanel();

        System.out.println("\n--- 场景 1: 成功输入 (1234) ---");
        for (char c : "1234".toCharArray()) panel.inputKey
        System.out.println("\n--- 场景 2: 连续错误输入 (1111, 2222, 0000) ---");
        for (char c : "1111".toCharArray()) panel.inputKey(c);
        for (char c : "2222".toCharArray()) panel.inputKey(c);
        for (char c : "0000".toCharArray()) panel.inputKey(c);

        System.out.println("\n--- 场景 3: 锁定期间尝试输入 (模拟 5 秒后) ---");
        Thread.sleep(5000);
        panel.inputKey('5');
    }
}