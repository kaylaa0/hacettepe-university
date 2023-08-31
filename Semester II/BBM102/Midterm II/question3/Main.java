package question3;

public class Main {
    public static void main(String []args){
        System.out.println("Enjoying the Enhanced Map");
        EnhancedMap<String,Integer> em = new EnhancedMap<String,Integer>();
        em.put("Ahmet", 15);
        em.put("Ahmet", 20);
        em.put("Ahmet", 67);
        System.out.println(em.getMean("Ahmet"));
        System.out.println(em.get("Ahmet"));
        System.out.println(em.getMean("Seda"));
    }
}
