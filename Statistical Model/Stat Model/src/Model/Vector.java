package Model;

import java.lang.Math;

/**
 *
 * @author abdullah.almosalami
 */
public class Vector {
    private double x, y;
    
    public Vector(){
        x=0;
        y=0;
    }
    public Vector(double x, double y){
        this.x = x;
        this.y = y;
    }
    
    protected double getVectorMag(){
        return Math.hypot(x, y);
    }
    protected double getDirection(){
        return Math.atan(y/x);  //works because returns value from -90 to 90 degrees
    }
    protected double getX(){
        return x;
    }
    protected double getY(){
        return y;
    }

    @Override
    public String toString() {
        return "Vector{" + "x=" + x + ", y=" + y + '}';
    }
}
