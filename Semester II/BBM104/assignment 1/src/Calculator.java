/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.List;

public class Calculator {
    public Calculator(List<Product> priceList, List<Entry> shoppingList) {
        for (int i = 0; i < shoppingList.size(); i++) {
            this.Calculate(priceList, shoppingList.get(i));
        }
    }

    public void Calculate(List<Product> priceList, Entry shoppingEntry) {
        for (int e = 0; e < shoppingEntry.items.size(); e++) {
            boolean found = false;
            for (int i = 0; i < priceList.size(); i++) {
                if (shoppingEntry.items.get(e).name.equals(priceList.get(i).name)
                        && shoppingEntry.type.equals(priceList.get(i).type)
                        && this.dateChecker(shoppingEntry.date, priceList.get(i).dateFrom, priceList.get(i).dateTo)) {
                    shoppingEntry.total += (Float.parseFloat(shoppingEntry.items.get(e).amount)
                            * (Float.parseFloat(priceList.get(i).price) * 100)) / 100;
                    shoppingEntry.items.get(e).setPrice(priceList.get(i).price);
                    found = true;
                }
            }
            if (!found) {
                System.out.println(
                        "Error: Item is not found in the price list, item name: " + shoppingEntry.items.get(e));
            }
        }
    }

    public boolean dateChecker(String checkText, String fromText, String toText) {
        Date check = new Date(checkText);
        Date from = new Date(fromText);
        Date to = new Date(toText);
        if ((check.year > from.year) && (check.year < to.year)) {
            return true;
        } else if ((check.year.equals(from.year)) && (check.year.equals(to.year))) {
            if ((check.month > from.month) && (check.month < to.month)) {
                return true;
            } else if ((check.month.equals(from.month)) && (check.month.equals(to.month))) {
                if ((check.day >= from.day) && (check.day <= to.day)) {
                    return true;
                }
                return false;
            } else if (check.month.equals(from.month)) {
                if (check.day >= from.day) {
                    return true;
                }
                return false;
            } else if (check.month.equals(to.month)) {
                if (check.day <= to.day) {
                    return true;
                }
                return false;
            }
            return false;
        } else if (check.year.equals(from.year)) {
            if (check.month > from.month) {
                return true;
            } else if (check.month.equals(from.month)) {
                if (check.day >= from.day) {
                    return true;
                }
                return false;
            }
            return false;
        } else if (check.year.equals(to.year)) {
            if (check.month < to.month) {
                return true;
            } else if (check.month.equals(to.month)) {
                if (check.day <= to.day) {
                    return true;
                }
                return false;
            }
            return false;
        }
        return false;
    }
}
