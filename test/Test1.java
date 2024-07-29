/**
 * Created with IntelliJ IDEA.
 * Author: Lettle
 * Date: 2024/7/30
 * Time: 1:25
 * Description:
 */

public class Test1 {
    public static void main(String[] args) {
        int a = 7+3;
        int b = a*6;
        int c = b/2;
        if (c == 30) {
            say("Hello, LettleJVM!!!");
        } else {
            say("Wrong result!!!");
        }
    }

    public static void say (String msg) {
        System.out.println(msg);
    }
}
