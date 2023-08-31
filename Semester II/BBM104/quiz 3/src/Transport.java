/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class Transport {
    private String name;
    private Integer speed;

    public Transport() {

    }

    public String getName() {
        return name;
    }

    public Integer estimateTime(String from, String to) {
        String response = this.executePost(
                "https://maps.googleapis.com/maps/api/directions/json?origin={id1}&destination={id2}&key={id3}",
                "id1=" + from + "id2=" + to + "id3=AIzaSyDv1gnyCV8t9GqJ9QdtURxoj2GYOqjPwAI");
        Integer time = 1;
        return time;
    }
}
