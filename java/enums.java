public class enums {

    enum Status {
        RUNNING, FAILED, SUCCESS, ERROR;
    };

    public static void main(String[] args) {
        Status s = Status.ERROR;

        switch (s) {
            case RUNNING:
                System.out.println("Process running");
                break;

            case FAILED:
                System.out.println("Process failed");
                break;

            case SUCCESS:
                System.out.println("Process success");
                break;
        
            default:
                System.out.println("Process exited with error");
                break;
        }
    }
}
