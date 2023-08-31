package question3;

import java.util.HashMap;

public class EnhancedMap<K,V> {
    HashMap<K, V[]> myHashMap = new HashMap<K, V[]>();

    public V[] put(K key, V value) {
        if (myHashMap.get(key) == null) {
            V[] temp = (V[]) new Object[]{value};
            return myHashMap.put(key, temp);
        } else {
            V[] temp = (V[]) new Object[myHashMap.get(key).length + 1];
            for (int i = 0; i < myHashMap.get(key).length; i++) {
                temp[i] = myHashMap.get(key)[i];
            }
            temp[myHashMap.get(key).length] = value;
            return myHashMap.put(key, temp);
        }
    }

    public String get(Object key) {
        if(myHashMap.get(key) == null){
            return "-1";
        }else{
            StringBuilder returnString = new StringBuilder("[");
            int[] temp = new int[myHashMap.get(key).length];
            for(int i = 0 ; i < myHashMap.get(key).length; i++){
                returnString.append((Integer) myHashMap.get(key)[i]).append(",");
            }
            returnString.delete(returnString.length()-1, returnString.length());
            returnString.append("]");
            return returnString.toString();
        }
    }

    public float getMean(Object key){
        if(myHashMap.get(key) == null){
            return -1;
        }else{int[] temp = new int[myHashMap.get(key).length];
            for(int i = 0 ; i < myHashMap.get(key).length; i++){
                temp[i] = (Integer) myHashMap.get(key)[i];
            }
            int[] intList = temp;
            int total = 0;
            for(int inte: intList){
                total += inte;
            }
            return total/myHashMap.get(key).length;}
    }
}
