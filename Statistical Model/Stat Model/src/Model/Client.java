package Model;

import java.util.ArrayList;
import java.lang.Math;

/**
 *
 * @author abdullah.almosalami
 */
public class Client {
    private static final double ACCURACY_OF_ANGLE_CHANGE = 0.5;    //IF THIS CHANGES, NEED TO MAKE CORRESPONDING CHANGE TO PossibilityTuple DEFAULT CONSTRUCTOR
    private static final double MAX_ANGLE_CHANGE = 20;  //maximum angle change on either side of 0
    private static final int TOTAL_NUMBER_OF_POSSIBILITIES_PER_TARGET_PER_INTERVAL = (int) Math.pow((MAX_ANGLE_CHANGE * 2) / ACCURACY_OF_ANGLE_CHANGE, 4);
    
    private static final double SPEED = 0.33;   //in meters per second
    private static final double TIME_INTERVAL = 5;  //in seconds
    
    private static final int GRID_SIZE_X = 30, GRID_SIZE_Y = 30;
    
    public static void main(String[] args){
        //Initial possibilities
        ArrayList<PossibilityTuple> bag_of_possible_tuples = generateInitPossibilities();
        ArrayList<Vector> bag_of_possible_vectors = generateInitVectors(bag_of_possible_tuples);
        int[][] possibility_grid = fillGrid(bag_of_possible_vectors);
        printGrid(possibility_grid);
    }
    
    
    
    
    private static ArrayList<PossibilityTuple> generatePossibilities(){
        ArrayList<PossibilityTuple> bag_of_tuples = new ArrayList<PossibilityTuple>(TOTAL_NUMBER_OF_POSSIBILITIES_PER_TARGET_PER_INTERVAL);
        
        for(double i=-MAX_ANGLE_CHANGE; i<=MAX_ANGLE_CHANGE; i+=ACCURACY_OF_ANGLE_CHANGE){
            for(double j=-MAX_ANGLE_CHANGE; j<=MAX_ANGLE_CHANGE; j+=ACCURACY_OF_ANGLE_CHANGE){
                for(double k=-MAX_ANGLE_CHANGE; k<=MAX_ANGLE_CHANGE; k+=ACCURACY_OF_ANGLE_CHANGE){
                    for(double l=-MAX_ANGLE_CHANGE; l<=MAX_ANGLE_CHANGE; l+=ACCURACY_OF_ANGLE_CHANGE){
                        PossibilityTuple possiblity = new PossibilityTuple(i, j, k, l);
                        bag_of_tuples.add(possiblity);
                    }
                }
            }
        }
        
        return bag_of_tuples;
    }
    private static ArrayList<PossibilityTuple> generateInitPossibilities(){
        ArrayList<PossibilityTuple> bag_of_tuples = new ArrayList<PossibilityTuple>(512000);
        
        for(double j=-MAX_ANGLE_CHANGE; j<=MAX_ANGLE_CHANGE; j+=ACCURACY_OF_ANGLE_CHANGE){
            for(double k=-MAX_ANGLE_CHANGE; k<=MAX_ANGLE_CHANGE; k+=ACCURACY_OF_ANGLE_CHANGE){
                for(double l=-MAX_ANGLE_CHANGE; l<=MAX_ANGLE_CHANGE; l+=ACCURACY_OF_ANGLE_CHANGE){
                    PossibilityTuple possiblity = new PossibilityTuple(0, j, k, l);
                    bag_of_tuples.add(possiblity);
                }
            }
        }
        
        return bag_of_tuples;
    }
    
    
    
    
    private static ArrayList<Vector> generateVectors(ArrayList<PossibilityTuple> bag_of_tuples, double xi, double yi){
        ArrayList<Vector> bag_of_vectors = new ArrayList<Vector>(TOTAL_NUMBER_OF_POSSIBILITIES_PER_TARGET_PER_INTERVAL);
        double delta_x=0, delta_y=0, xf=xi, yf=yi;
        
        //ACCOUNT FOR COMPOUNDED ANGLES AND ADD TIME_INTERVAL CONSTANT!
        //ALSO ACCOUNT FOR INITIAL POSITION OF OBJECT!
        for(PossibilityTuple e : bag_of_tuples){
            delta_x = SPEED * TIME_INTERVAL * (Math.cos(Math.toRadians(e.getE1())) + Math.cos(Math.toRadians(e.getE1() + e.getE2())) + Math.cos(Math.toRadians(e.getE1() + e.getE2() + e.getE3())) + Math.cos(Math.toRadians(e.getE1() + e.getE2() + e.getE3() + e.getE4())));
            delta_y = SPEED * TIME_INTERVAL * (Math.sin(Math.toRadians(e.getE1())) + Math.sin(Math.toRadians(e.getE1() + e.getE2())) + Math.sin(Math.toRadians(e.getE1() + e.getE2() + e.getE3())) + Math.sin(Math.toRadians(e.getE1() + e.getE2() + e.getE3() + e.getE4())));
            xf = xi + delta_x;
            yf = yi + delta_y;
            Vector newVector = new Vector(xf, yf);
            bag_of_vectors.add(newVector);
        }
        
        return bag_of_vectors;
    }
    private static ArrayList<Vector> generateInitVectors(ArrayList<PossibilityTuple> bag_of_tuples){
        return generateVectors(bag_of_tuples, 15.5, 15);    //15.5 AND 15 REPRESENT THE INITIAL POSITION OF TARGET 1
    }
    
    
    
    
    private static int[][] fillGrid(ArrayList<Vector> bag_of_vectors){
        int[][] grid = new int[GRID_SIZE_X][GRID_SIZE_Y];   //BECAUSE IT'S A NEW GRID EACH TIME, CALLING THIS METHOD IS A RESET IN OF ITSELF
        //Initialize 2D Array
        for(int i=0; i<GRID_SIZE_X; i++){
            for(int j=0; j<GRID_SIZE_Y; j++){
                grid[i][j] = 0;
            }
        }
        
        for(Vector v : bag_of_vectors){
            int x = (int) Math.floor(v.getX());
            int y = (int) Math.floor(v.getY());
            
            grid[y][x]++;
        }
        
        return grid;
    }
    
    
    
    
    private static void printGrid(int[][] grid){
        for(int i=0; i<grid.length; i++){
            for(int j=0; j<grid[0].length; j++){
                System.out.format("%10d", grid[i][j]);
            }
            for(int k=0; k<4; k++){    //4 REPRESENTS THE VERTICAL SPACE BETWEEN NUMBERS
                System.out.println();
            }
        }
    }
}
