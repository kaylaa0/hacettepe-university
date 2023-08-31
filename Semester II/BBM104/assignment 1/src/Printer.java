/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.List;

public class Printer {
    public Printer(List<Entry> shoppingList) {
        for (int i = 0; i < shoppingList.size(); i++) {
            System.out.print("---" + shoppingList.get(i).customer + "---\n");
            for (int x = 0; x < shoppingList.get(i).items.size(); x++) {
                System.out.print(shoppingList.get(i).items.get(x).name + "\t"
                        + shoppingList.get(i).items.get(x).price.floatValue() + "\t"
                        + shoppingList.get(i).items.get(x).amount + "\t"
                        + ((shoppingList.get(i).items.get(x).price.floatValue() * 100)
                                * (Float.parseFloat(shoppingList.get(i).items.get(x).amount))) / 100
                        + "\n");
            }
            System.out.print("Total" + "\t" + shoppingList.get(i).total.floatValue() + "\n");
        }
    }
}
