interface Device {
    void work();
}

class Laptop implements Device {
    public void work() {
        System.out.println("Working on a laptop and\ndeveloping an app");
    }
};

class Desktop implements Device {
    public void work() {
        System.out.println("Working on a desktop and\ndeveloping an app");
    }
};

class Developer {
    public void work(Device device) {
        // System.out.println("Developing an application");
        device.work();
    }
};

public class work {
    
    public static void main(String args[]) {

        Developer dev = new Developer();
        // Laptop laptop = new Laptop();
        // dev.work(laptop);

        // Desktop desktop = new Desktop();
        // dev.work(desktop);

        Device d1 = new Laptop();
        dev.work(d1);

        Device d2 = new Desktop();
        dev.work(d2);
    }
}
