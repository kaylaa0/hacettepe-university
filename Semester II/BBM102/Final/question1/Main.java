public class Main {
    class Animal {
        public void method1() {
            System.out.println(2);
        }
    }

    class Mammal extends Animal {
        public void method1() {
            System.out.println(1);
        }

        public void method2() {
            method1();
            System.out.println("method 2 mammal");
        }
    }

    class Cat extends Mammal {
        public void method1() {
            System.out.println(3);
        }

        public void method3() {
            method1();
            System.out.println("method 2 cat");
        }
    }

    public void start() {
        Object o = new Object();
        Animal a = new Animal();
        o = a;
        ((Animal) o).method1();
        System.out.println(Math.pow(16, (1.0 / 4)));
    }

    public static void main(String[] args) {
        Main program = new Main();
        program.start();
    }
}
