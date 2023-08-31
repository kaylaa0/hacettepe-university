package question2;

public class Main {
    public double f(double number) throws ExceptionA, ExceptionB, RuntimeException{
        return Math.sqrt(number);
    }

    public double g(double number){
        try{
            return f(number)*2;
        }
        catch(ExceptionA e){
            return -1;
        }catch(ExceptionC e){
            return 1;
        }catch (RunTimeException e){
            return 1;
        }
    }
}
