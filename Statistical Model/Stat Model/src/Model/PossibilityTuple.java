package Model;

import java.util.Random;

/**
 *
 * @author abdullah.almosalami
 */
public class PossibilityTuple {
    private double[] quadTuple = new double[4];
    private Random rand;
    
    public PossibilityTuple(){
        for(int i=0; i<4; i++){
            quadTuple[i] = rand.nextInt(41)*0.5 - 10;
        }
    }
    public PossibilityTuple(double e1, double e2, double e3, double e4){
        quadTuple[0] = e1;
        quadTuple[1] = e2;
        quadTuple[2] = e3;
        quadTuple[3] = e4;
    }
    
    protected double[] getTuple(){
        return quadTuple;
    }
    protected double getE1(){
        return quadTuple[0];
    }
    protected double getE2(){
        return quadTuple[1];
    }
    protected double getE3(){
        return quadTuple[2];
    }
    protected double getE4(){
        return quadTuple[3];
    }

    @Override
    public String toString() {
        String returnString = "(";
        for(double i : quadTuple){
            returnString += i + ", ";
        }
        returnString += ")";
        
        return returnString;
    }
}
