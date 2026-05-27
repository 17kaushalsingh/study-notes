class Calculator {
    private final double INF = 1e8;

    public double add(double num1, double num2) {
        return (num1 + num2);
    }

    public double subtract(double num1, double num2) {
        return (num1 - num2);
    }

    public double multiply(double num1, double num2) {
        return (num1 * num2);
    }

    public double divide(double num1, double num2) {
        if (num2 != 0.0) return (num1/num2);
        return (num1 >= 0) ? (INF) : (-INF);
    }

    public void showDetails() {
        System.out.println("In class Calculator");
    }
};

class ScientificCalculator extends Calculator {
    public double exp(double a, double b) {
        return Math.pow(a, b);
    }

    public void showDetails() {
        System.out.println("In class ScientificCalculator");
    }
};

public class oops {

    public static void main(String args[]) {
        // Calculator calc = new Calculator();
        // double r1 = calc.add(34.5, 19);
        // System.out.println(r1);
        // System.out.println(calc);

        // calc.showDetails();

        // ScientificCalculator sciCalc = new ScientificCalculator();
        // double r2 = sciCalc.exp(2, 3);
        // System.out.println(r2);

        // System.out.println("Are calc and sciCalc equal? " + (calc == sciCalc));
        // sciCalc.showDetails();

        Calculator obj = new Calculator();
        obj.showDetails();

        Calculator obj1 = new ScientificCalculator();
        System.out.println(obj1.add(2, 3));
        obj1.showDetails();
    }
}
