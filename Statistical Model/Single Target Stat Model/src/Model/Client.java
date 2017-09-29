package Model;

import java.util.ArrayList;
import java.lang.Math;

/**
 *
 * @author abdullah.almosalami
 */
public class Client {
    private static final double INCREMENT = 0.5;    //IF THIS CHANGES, NEED TO MAKE CORRESPONDING CHANGE TO PossibilityTuple DEFAULT CONSTRUCTOR
    private static final double SPEED = 0.33;
    
    public static void main(String[] args){
        //Initial possibilities
        ArrayList<PossibilityTuple> bag_of_possible_tuples = generateInitPossibilities();
        ArrayList<Vector> bag_of_possible_vectors = generateInitVectors(bag_of_possible_tuples);
        int[][] possibility_grid = fillGrid(bag_of_possible_vectors);
        printGrid(possibility_grid);
    }
    
    private static ArrayList<PossibilityTuple> generatePossibilities(){
        ArrayList<PossibilityTuple> bag_of_tuples = new ArrayList<PossibilityTuple>(2560000);
        
        for(double i=-10; i<=10; i=i+INCREMENT){
            for(double j=-10; j<=10; j=j+INCREMENT){
                for(double k=-10; k<=10; k=k+INCREMENT){
                    for(double l=-10; l<=10; l=l+INCREMENT){
                        PossibilityTuple possiblity = new PossibilityTuple(i, j, k, l);
                        bag_of_tuples.add(possiblity);
                    }
                }
            }
        }
        
        return bag_of_tuples;
    }
    private static ArrayList<PossibilityTuple> generateInitPossibilities(){
        ArrayList<PossibilityTuple> bag_of_tuples = new ArrayList<PossibilityTuple>(64000);
        
        for(double i=-10; i<=10; i=i+INCREMENT){
            for(double j=-10; j<=10; j=j+INCREMENT){
                for(double k=-10; k<=10; k=k+INCREMENT){
                    PossibilityTuple possiblity = new PossibilityTuple(0, i, j, k);
                    bag_of_tuples.add(possiblity);
                }
            }
        }
        
        return bag_of_tuples;
    }
    
    private static ArrayList<Vector> generateVectors(ArrayList<PossibilityTuple> bag_of_tuples){
        ArrayList<Vector> bag_of_vectors = new ArrayList<Vector>(2560000);
        double x, y;
        
        //ACCOUNT FOR COMPOUNDED ANGLES AND ADD TIME_INTERVAL CONSTANT!
        //ALSO ACCOUNT FOR INITIAL POSITION OF OBJECT!
        for(PossibilityTuple e : bag_of_tuples){
            x = SPEED * (Math.cos(e.getE1()) + Math.cos(e.getE2()) + Math.cos(e.getE3()) + Math.cos(e.getE4()));
            y = SPEED * (Math.sin(e.getE1()) + Math.sin(e.getE2()) + Math.sin(e.getE3()) + Math.sin(e.getE4()));
            Vector newVector = new Vector(x, y);
            bag_of_vectors.add(newVector);
        }
        
        return bag_of_vectors;
    }
    private static ArrayList<Vector> generateInitVectors(ArrayList<PossibilityTuple> bag_of_tuples){
        ArrayList<Vector> bag_of_vectors = new ArrayList<Vector>(64000);
        double x, y;
        
        for(PossibilityTuple e : bag_of_tuples){
            x = SPEED * (1 + Math.cos(e.getE2()) + Math.cos(e.getE3()) + Math.cos(e.getE4()));
            y = SPEED * (1 + Math.sin(e.getE2()) + Math.sin(e.getE3()) + Math.sin(e.getE4()));
            Vector newVector = new Vector(x, y);
            bag_of_vectors.add(newVector);
        }
        
        return bag_of_vectors;
    }
    
    private static int[][] fillGrid(ArrayList<Vector> bag_of_vectors){
        //MAKE GRID THE ACTUAL COMPLETE FULL 30X30M GRID AND SET (0,0) AS THE BOTTOM LEFT CORNER
        int[][] grid = new int[20][9];
        //Initialize 2D Array
        for(int i=0; i<20; i++){
            for(int j=0; j<9; j++){
                grid[i][j] = 0;
            }
        }
        
        for(Vector v : bag_of_vectors){
            double x = v.getX();
            double y = v.getY();
            
            //ELIMINIATE ALL IF STATEMENTS BY USING MATH.FLOOR() METHOD!
            if(x>=0 && x<1){
                if(y>=0 && y<1){
                    grid[0][0]++;
                }
                if(y>=1 && y<2){
                    grid[1][0]++;
                }
                if(y>=2 && y<3){
                    grid[2][0]++;
                }
                if(y>=3 && y<4){
                    grid[3][0]++;
                }
                if(y>=4 && y<5){
                    grid[4][0]++;
                }
                if(y>=5 && y<6){
                    grid[5][0]++;
                }
                if(y>=6 && y<7){
                    grid[6][0]++;
                }
                if(y>=7 && y<8){
                    grid[7][0]++;
                }
                if(y>=8 && y<9){
                    grid[8][0]++;
                }
                if(y>=9 && y<10){
                    grid[9][0]++;
                }
                if(y>=10 && y<11){
                    grid[10][0]++;
                }
                if(y>=11 && y<12){
                    grid[11][0]++;
                }
                if(y>=12 && y<13){
                    grid[12][0]++;
                }
                if(y>=13 && y<14){
                    grid[13][0]++;
                }
                if(y>=14 && y<15){
                    grid[14][0]++;
                }
                if(y>=15 && y<16){
                    grid[15][0]++;
                }
                if(y>=16 && y<17){
                    grid[16][0]++;
                }
                if(y>=17 && y<18){
                    grid[17][0]++;
                }
                if(y>=18 && y<19){
                    grid[18][0]++;
                }
                if(y>=19 && y<=20){
                    grid[19][0]++;
                }
            }
            if(x>=1 && x<2){
                if(y>=0 && y<1){
                    grid[0][1]++;
                }
                if(y>=1 && y<2){
                    grid[1][1]++;
                }
                if(y>=2 && y<3){
                    grid[2][1]++;
                }
                if(y>=3 && y<4){
                    grid[3][1]++;
                }
                if(y>=4 && y<5){
                    grid[4][1]++;
                }
                if(y>=5 && y<6){
                    grid[5][1]++;
                }
                if(y>=6 && y<7){
                    grid[6][1]++;
                }
                if(y>=7 && y<8){
                    grid[7][1]++;
                }
                if(y>=8 && y<9){
                    grid[8][1]++;
                }
                if(y>=9 && y<10){
                    grid[9][1]++;
                }
                if(y>=10 && y<11){
                    grid[10][1]++;
                }
                if(y>=11 && y<12){
                    grid[11][1]++;
                }
                if(y>=12 && y<13){
                    grid[12][1]++;
                }
                if(y>=13 && y<14){
                    grid[13][1]++;
                }
                if(y>=14 && y<15){
                    grid[14][1]++;
                }
                if(y>=15 && y<16){
                    grid[15][1]++;
                }
                if(y>=16 && y<17){
                    grid[16][1]++;
                }
                if(y>=17 && y<18){
                    grid[17][1]++;
                }
                if(y>=18 && y<19){
                    grid[18][1]++;
                }
                if(y>=19 && y<=20){
                    grid[19][1]++;
                }
            }
            if(x>=2 && x<3){
                if(y>=0 && y<1){
                    grid[0][2]++;
                }
                if(y>=1 && y<2){
                    grid[1][2]++;
                }
                if(y>=2 && y<3){
                    grid[2][2]++;
                }
                if(y>=3 && y<4){
                    grid[3][2]++;
                }
                if(y>=4 && y<5){
                    grid[4][2]++;
                }
                if(y>=5 && y<6){
                    grid[5][2]++;
                }
                if(y>=6 && y<7){
                    grid[6][2]++;
                }
                if(y>=7 && y<8){
                    grid[7][2]++;
                }
                if(y>=8 && y<9){
                    grid[8][2]++;
                }
                if(y>=9 && y<10){
                    grid[9][2]++;
                }
                if(y>=10 && y<11){
                    grid[10][2]++;
                }
                if(y>=11 && y<12){
                    grid[11][2]++;
                }
                if(y>=12 && y<13){
                    grid[12][2]++;
                }
                if(y>=13 && y<14){
                    grid[13][2]++;
                }
                if(y>=14 && y<15){
                    grid[14][2]++;
                }
                if(y>=15 && y<16){
                    grid[15][2]++;
                }
                if(y>=16 && y<17){
                    grid[16][2]++;
                }
                if(y>=17 && y<18){
                    grid[17][2]++;
                }
                if(y>=18 && y<19){
                    grid[18][2]++;
                }
                if(y>=19 && y<=20){
                    grid[19][2]++;
                }
            }
            if(x>=3 && x<4){
                if(y>=0 && y<1){
                    grid[0][3]++;
                }
                if(y>=1 && y<2){
                    grid[1][3]++;
                }
                if(y>=2 && y<3){
                    grid[2][3]++;
                }
                if(y>=3 && y<4){
                    grid[3][3]++;
                }
                if(y>=4 && y<5){
                    grid[4][3]++;
                }
                if(y>=5 && y<6){
                    grid[5][3]++;
                }
                if(y>=6 && y<7){
                    grid[6][3]++;
                }
                if(y>=7 && y<8){
                    grid[7][3]++;
                }
                if(y>=8 && y<9){
                    grid[8][3]++;
                }
                if(y>=9 && y<10){
                    grid[9][3]++;
                }
                if(y>=10 && y<11){
                    grid[10][3]++;
                }
                if(y>=11 && y<12){
                    grid[11][3]++;
                }
                if(y>=12 && y<13){
                    grid[12][3]++;
                }
                if(y>=13 && y<14){
                    grid[13][3]++;
                }
                if(y>=14 && y<15){
                    grid[14][3]++;
                }
                if(y>=15 && y<16){
                    grid[15][3]++;
                }
                if(y>=16 && y<17){
                    grid[16][3]++;
                }
                if(y>=17 && y<18){
                    grid[17][3]++;
                }
                if(y>=18 && y<19){
                    grid[18][3]++;
                }
                if(y>=19 && y<=20){
                    grid[19][3]++;
                }
            }
            if(x>=4 && x<5){
                if(y>=0 && y<1){
                    grid[0][4]++;
                }
                if(y>=1 && y<2){
                    grid[1][4]++;
                }
                if(y>=2 && y<3){
                    grid[2][4]++;
                }
                if(y>=3 && y<4){
                    grid[3][4]++;
                }
                if(y>=4 && y<5){
                    grid[4][4]++;
                }
                if(y>=5 && y<6){
                    grid[5][4]++;
                }
                if(y>=6 && y<7){
                    grid[6][4]++;
                }
                if(y>=7 && y<8){
                    grid[7][4]++;
                }
                if(y>=8 && y<9){
                    grid[8][4]++;
                }
                if(y>=9 && y<10){
                    grid[9][4]++;
                }
                if(y>=10 && y<11){
                    grid[10][4]++;
                }
                if(y>=11 && y<12){
                    grid[11][4]++;
                }
                if(y>=12 && y<13){
                    grid[12][4]++;
                }
                if(y>=13 && y<14){
                    grid[13][4]++;
                }
                if(y>=14 && y<15){
                    grid[14][4]++;
                }
                if(y>=15 && y<16){
                    grid[15][4]++;
                }
                if(y>=16 && y<17){
                    grid[16][4]++;
                }
                if(y>=17 && y<18){
                    grid[17][4]++;
                }
                if(y>=18 && y<19){
                    grid[18][4]++;
                }
                if(y>=19 && y<=20){
                    grid[19][4]++;
                }
            }
            if(x>=5 && x<6){
                if(y>=0 && y<1){
                    grid[0][5]++;
                }
                if(y>=1 && y<2){
                    grid[1][5]++;
                }
                if(y>=2 && y<3){
                    grid[2][5]++;
                }
                if(y>=3 && y<4){
                    grid[3][5]++;
                }
                if(y>=4 && y<5){
                    grid[4][5]++;
                }
                if(y>=5 && y<6){
                    grid[5][5]++;
                }
                if(y>=6 && y<7){
                    grid[6][5]++;
                }
                if(y>=7 && y<8){
                    grid[7][5]++;
                }
                if(y>=8 && y<9){
                    grid[8][5]++;
                }
                if(y>=9 && y<10){
                    grid[9][5]++;
                }
                if(y>=10 && y<11){
                    grid[10][5]++;
                }
                if(y>=11 && y<12){
                    grid[11][5]++;
                }
                if(y>=12 && y<13){
                    grid[12][5]++;
                }
                if(y>=13 && y<14){
                    grid[13][5]++;
                }
                if(y>=14 && y<15){
                    grid[14][5]++;
                }
                if(y>=15 && y<16){
                    grid[15][5]++;
                }
                if(y>=16 && y<17){
                    grid[16][5]++;
                }
                if(y>=17 && y<18){
                    grid[17][5]++;
                }
                if(y>=18 && y<19){
                    grid[18][5]++;
                }
                if(y>=19 && y<=20){
                    grid[19][5]++;
                }
            }
            if(x>=6 && x<7){
                if(y>=0 && y<1){
                    grid[0][6]++;
                }
                if(y>=1 && y<2){
                    grid[1][6]++;
                }
                if(y>=2 && y<3){
                    grid[2][6]++;
                }
                if(y>=3 && y<4){
                    grid[3][6]++;
                }
                if(y>=4 && y<5){
                    grid[4][6]++;
                }
                if(y>=5 && y<6){
                    grid[5][6]++;
                }
                if(y>=6 && y<7){
                    grid[6][6]++;
                }
                if(y>=7 && y<8){
                    grid[7][6]++;
                }
                if(y>=8 && y<9){
                    grid[8][6]++;
                }
                if(y>=9 && y<10){
                    grid[9][6]++;
                }
                if(y>=10 && y<11){
                    grid[10][6]++;
                }
                if(y>=11 && y<12){
                    grid[11][6]++;
                }
                if(y>=12 && y<13){
                    grid[12][6]++;
                }
                if(y>=13 && y<14){
                    grid[13][6]++;
                }
                if(y>=14 && y<15){
                    grid[14][6]++;
                }
                if(y>=15 && y<16){
                    grid[15][6]++;
                }
                if(y>=16 && y<17){
                    grid[16][6]++;
                }
                if(y>=17 && y<18){
                    grid[17][6]++;
                }
                if(y>=18 && y<19){
                    grid[18][6]++;
                }
                if(y>=19 && y<=20){
                    grid[19][6]++;
                }
            }
            if(x>=7 && x<8){
                if(y>=0 && y<1){
                    grid[0][7]++;
                }
                if(y>=1 && y<2){
                    grid[1][7]++;
                }
                if(y>=2 && y<3){
                    grid[2][7]++;
                }
                if(y>=3 && y<4){
                    grid[3][7]++;
                }
                if(y>=4 && y<5){
                    grid[4][7]++;
                }
                if(y>=5 && y<6){
                    grid[5][7]++;
                }
                if(y>=6 && y<7){
                    grid[6][7]++;
                }
                if(y>=7 && y<8){
                    grid[7][7]++;
                }
                if(y>=8 && y<9){
                    grid[8][7]++;
                }
                if(y>=9 && y<10){
                    grid[9][7]++;
                }
                if(y>=10 && y<11){
                    grid[10][7]++;
                }
                if(y>=11 && y<12){
                    grid[11][7]++;
                }
                if(y>=12 && y<13){
                    grid[12][7]++;
                }
                if(y>=13 && y<14){
                    grid[13][7]++;
                }
                if(y>=14 && y<15){
                    grid[14][7]++;
                }
                if(y>=15 && y<16){
                    grid[15][7]++;
                }
                if(y>=16 && y<17){
                    grid[16][7]++;
                }
                if(y>=17 && y<18){
                    grid[17][7]++;
                }
                if(y>=18 && y<19){
                    grid[18][7]++;
                }
                if(y>=19 && y<=20){
                    grid[19][7]++;
                }
            }
            if(x>=8 && x<=9){
                if(y>=0 && y<1){
                    grid[0][8]++;
                }
                if(y>=1 && y<2){
                    grid[1][8]++;
                }
                if(y>=2 && y<3){
                    grid[2][8]++;
                }
                if(y>=3 && y<4){
                    grid[3][8]++;
                }
                if(y>=4 && y<5){
                    grid[4][8]++;
                }
                if(y>=5 && y<6){
                    grid[5][8]++;
                }
                if(y>=6 && y<7){
                    grid[6][8]++;
                }
                if(y>=7 && y<8){
                    grid[7][8]++;
                }
                if(y>=8 && y<9){
                    grid[8][8]++;
                }
                if(y>=9 && y<10){
                    grid[9][8]++;
                }
                if(y>=10 && y<11){
                    grid[10][8]++;
                }
                if(y>=11 && y<12){
                    grid[11][8]++;
                }
                if(y>=12 && y<13){
                    grid[12][8]++;
                }
                if(y>=13 && y<14){
                    grid[13][8]++;
                }
                if(y>=14 && y<15){
                    grid[14][8]++;
                }
                if(y>=15 && y<16){
                    grid[15][8]++;
                }
                if(y>=16 && y<17){
                    grid[16][8]++;
                }
                if(y>=17 && y<18){
                    grid[17][8]++;
                }
                if(y>=18 && y<19){
                    grid[18][8]++;
                }
                if(y>=19 && y<=20){
                    grid[19][8]++;
                }
            }
        }
        
        System.out.println(grid[10][5] + "\n\n");
        
        return grid;
    }
    
    
    private static void printGrid(int[][] grid){
        for(int i=0; i<grid.length; i++){
            for(int j=0; j<grid[0].length; j++){
                System.out.format("%10d", grid[i][j]);
            }
            System.out.println();
        }
    }
}
