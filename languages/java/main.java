
class Calculator {

    public int add(int x, int y) {
        int ans = x + y;
        return ans;
    }
};

class main{
    public static void main(String[] args) {
        System.out.println("Hello world");

        int num1 = 7;
        System.out.println(num1);

        int age = 14;
        if (age >= 18) {
            System.out.println("Adult person");
        } else {
            System.out.println("Minor person");
        }

        System.out.println("---------------------");
        System.out.println("---------------------");
        System.out.println("Calculator using OOPs");
        System.out.println("---------------------");
        System.out.println("---------------------");

        int x = 4;
        int y = 45;

        Calculator calc = new Calculator();

        System.out.println("Sum of x: " + x + " and y: " + y + " is: " + calc.add(x, y));

        int nums[] = {1,4,6,8};
        System.out.println(nums[0]);
    }
}

