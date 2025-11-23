import java.util.ArrayList;
import java.util.List;

public class StringExtractor {

    public enum State {
        WAITING("等待读取状态"),
        READING_FILE("读取文件状态"),
        READING_STRING("读取字符串状态"),
        HANDLE_ESCAPE("处理转义字符状态");

        private final String value;

        State(String value) {
            this.value = value;
        }

        public String getValue() {
            return value;
        }
    }

    private State state;
    private StringBuilder currentString;
    private List<String> extractedStrings;

    public StringExtractor() {
        this.state = State.WAITING;
        this.currentString = new StringBuilder();
        this.extractedStrings = new ArrayList<>();
    }

    public List<String> extract(String sourceText) {
        this.state = State.READING_FILE;
        this.extractedStrings.clear();
        this.currentString.setLength(0);

        System.out.println("--- 字符串提取 FSM 启动 ---");

        for (char c : sourceText.toCharArray()) {
            State currentState = this.state;

            if (currentState == State.READING_FILE) {
                if (c == '"') {
                    this.state = State.READING_STRING;
                }
            }
            else if (currentState == State.READING_STRING) {
                if (c == '"') {
                    this.extractedStrings.add(this.currentString.toString());
                    this.currentString.setLength(0);
                    this.state = State.READING_FILE;
                } else if (c == '\\') {
                    this.state = State.HANDLE_ESCAPE;
                } else {
                    this.currentString.append(c);
                }
            }
            else if (currentState == State.HANDLE_ESCAPE) {
                this.currentString.append(c);
                this.state = State.READING_STRING;
            }
        }

        if (this.state == State.READING_STRING) {
            System.out.println("\n[警告] 文件读取结束，但字符串未闭合！");
        }

        System.out.println("\n提取完成。最终状态: " + this.state.getValue());
        return this.extractedStrings;
    }

    public static void main(String[] args) {
        StringExtractor extractor = new StringExtractor();
        String testCase = "const char* s = \"一般学习一门编程语言的时候，基本上第一个程序是输出\\\"Hello World!\\\"\", end_line;";

        List<String> result = extractor.extract(testCase);
        System.out.println(result);
    }
}