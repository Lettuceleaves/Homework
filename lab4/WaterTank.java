import java.util.EnumSet;

public class WaterTank {

    public enum State {
        SLEEPING("休眠状态 (晚上 23:00 - 07:00)"),
        STANDBY("待机状态 (保温或等待加热)"),
        HEATING("加热状态 (电源开启)"),
        REPAIR("维修状态 (水箱烧坏)");

        private final String description;

        State(String description) {
            this.description = description;
        }

        public String getValue() {
            return description;
        }
    }

    private State state;
    private double temperature;
    private boolean waterLevel;
    private int currentTimeHour;

    public WaterTank() {
        this.state = State.SLEEPING;
        this.temperature = 25.0;
        this.waterLevel = true;
        this.currentTimeHour = 7;

        System.out.println("--- 恒温水箱 FSM 启动 ---");
        checkState();
    }

    public void setTime(int hour) {
        this.currentTimeHour = hour;
        System.out.printf("\n[事件] 时间更新至 %d:00%n", hour);
        checkState();
    }

    public void setSensors(double temp, boolean waterPresent) {
        this.temperature = temp;
        this.waterLevel = waterPresent;
        System.out.printf("[事件] 传感器更新: 温度=%.1f°C, 水量=%s%n", temp, waterPresent ? "有水" : "无水");
        checkState();
    }

    public void malfunction() {
        if (this.state != State.REPAIR) {
            System.out.printf("[转换] %s -> %s%n", this.state.name(), State.REPAIR.name());
            System.out.println("[动作] 警告：水箱烧坏！停止所有操作。");
            this.state = State.REPAIR;
        } else {
            System.out.println("水箱已在维修中。");
        }
    }

    public void repairDone() {
        if (this.state == State.REPAIR) {
            System.out.printf("[转换] %s -> %s%n", this.state.name(), State.STANDBY.name());
            System.out.println("[动作] 维修完成，系统进入待机。");
            this.state = State.STANDBY;
        } else {
            System.out.println("水箱未处于维修状态。");
        }
    }

    private void checkState() {
        State newState = this.state;

        if (this.state == State.REPAIR) {
            return;
        }
        if (this.currentTimeHour == 23 && this.state != State.SLEEPING) {
            newState = State.SLEEPING;
        } else if (this.currentTimeHour == 7 && this.state == State.SLEEPING) {
            newState = State.STANDBY;
        }

        if (newState == State.STANDBY || newState == State.HEATING) {
            if (this.state == State.HEATING && this.temperature >= 100) {
                newState = State.STANDBY;
                System.out.println("[动作] 达到 100°C，断开电源。");
            }
            else if (this.temperature < 20) {
                if (this.waterLevel) {
                    newState = State.HEATING;
                    System.out.println("[动作] 低于 20°C 且有水，打开继电器电源。");
                } else {
                    newState = State.STANDBY;
                    System.out.println("[动作] 低于 20°C 但无水，不打开电源 (保持待机)。");
                }
            }
            else if (this.temperature >= 20 && this.temperature < 100 && this.state == State.HEATING) {
                newState = State.HEATING;
            }
        }

        if (newState != this.state) {
            System.out.printf("[转换] %s -> %s (%s)%n", this.state.name(), newState.name(), newState.getValue());
            this.state = newState;
        } else {
            System.out.printf("当前状态: %s。未发生转换。%n", this.state.getValue());
        }
    }

    public static void main(String[] args) {
        WaterTank tank = new WaterTank();

        tank.setTime(7);
        tank.setSensors(25, true);

        tank.setSensors(15, true);

        tank.setSensors(100, true);

        tank.setSensors(10, false);

        tank.setTime(23);

        tank.setTime(10);
        tank.malfunction();
        tank.repairDone();
    }
}