/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    public static List<String> shoppingListText = new ArrayList<String>();
    public static List<String> priceListText = new ArrayList<String>();
    public static List<Product> priceList = new ArrayList<Product>();
    public static List<Entry> shoppingList = new ArrayList<Entry>();

    private static void readFile(String[] args) {
        Main.readFile(args[0], Main.shoppingListText);
        Main.readFile(args[1], Main.priceListText);
        initPriceList();
        initShoppingList();
    }

    private static void readFile(String filename, List<String> toread) {
        toread.clear();
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(filename))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
        }
        for (int i = 0; i < result.size(); i++) {
            toread.add(i, result.get(i));
        }
    }

    private static void initPriceList() {
        for (int i = 0; i < Main.priceListText.size(); i++) {
            priceList.add(i, new Product(priceListText.get(i)));
        }
    }

    private static void initShoppingList() {
        for (int i = 0; i < Main.shoppingListText.size(); i++) {
            shoppingList.add(i, new Entry(shoppingListText.get(i)));
        }
    }

    public static void main(String[] args) {
        Main.readFile(args);
        new Calculator(priceList, shoppingList);
        new Printer(shoppingList);
    }
}
