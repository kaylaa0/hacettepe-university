/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Phone {
    public String countryCode;
    public int code;
    public int number;
    public String type;

    public Phone(String countryCode, int code, int number, String type) {
        this.countryCode = countryCode;
        this.code = code;
        this.number = number;
        this.type = type;
    }

    public Phone(int code, int number, String type) {
        this.countryCode = "+90";
        this.code = code;
        this.number = number;
        this.type = type;
    }

    public String getCountryCode() {
        return countryCode;
    }

    public int getCode() {
        return code;
    }

    public int getNumber() {
        return number;
    }

    public String getType() {
        return type;
    }

    public void setCountryCode(String countryCode) {
        this.countryCode = countryCode;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public void setNumber(int number) {
        this.number = number;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void display() {
        System.out.println(type + " Phone: " + countryCode + " " + code + " " + number);
    }
}
