package Model;

import java.lang.Math;

/**
 *
 * @author abdullah.almosalami
 */
public class Vector {
    private double x, y, final_angle;
    
    public Vector(){
        x=0;
        y=0;
        final_angle = 0;
    }
    public Vector(double x, double y, double final_angle){
        this.x = x;
        this.y = y;
        this.final_angle = final_angle;
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
    protected double getFinalAngle(){
        return final_angle;
    }

    @Override
    public String toString() {
        return "Vector{" + "x=" + x + ", y=" + y + ", final_angle=" + final_angle + '}';
    }
}
