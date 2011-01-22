import random
import os

class Maze:  
    def __init__(self, size):
        self.size = size
        self.matrix = {}
        self.cells_checked = {}
        self.disp_matrix = {}
        self.path = []
        for row in range(1,(size+1)):
            for col in range(1,(size+1)):
                self.matrix[row,col] = ""
                self.cells_checked[row,col] = 0
                self.disp_matrix[row,col] = 0

    def block(self,row,col):
        if row > self.size or col > self.size:
            print "Block value ",row,",",col," is out of range"
            os._exit(0)
        self.matrix[row,col] = 0
        self.cells_checked[row,col] = 1
        

    def random_block(self):
        no_cells = self.size * self.size
        no_blockcells = random.randrange(1,(no_cells + 1))
        for blocks in range(1,(no_blockcells+1)):
            rand_row = random.randrange(1,(self.size+1))
            rand_col = random.randrange(1,(self.size+1))
            if self.matrix[rand_row,rand_col] == 0:
                continue
            else:
                self.matrix[rand_row,rand_col] = 0
                self.cells_checked[rand_row,rand_col] = 1

    def fillup(self):
        for rows in range(1,(self.size+1)):
            for cols in range(1,(self.size+1)):
                if self.matrix[rows,cols] != 0:
                    self.matrix[rows,cols] = 1

    def pathout(self,row,col):
        if self.matrix[self.size,1] == 0:
            print "There is no path : Entrance is blocked"
            os._exit(0)
        elif self.matrix[1,self.size] == 0:
            print "There is no path : Exit is blocked"
            os._exit(0)
        if row == 1 and col == self.size:
            print "path found"
            self.path.append([row,col])
            return 1
        if self.matrix[row,col] == 1:
            self.cells_checked[row,col] = 1
            self.path.append([row,col])
            if (row-1) <= self.size and (row-1) >= 1 and self.cells_checked[row-1,col] != 1:
                self.pathout(row-1,col)
            elif (row+1) <= self.size and (row+1) >= 1 and self.cells_checked[row+1,col] != 1:
                self.pathout(row+1,col)
            elif (col-1) <= self.size and (col-1) >= 1 and self.cells_checked[row,col-1] != 1:
                self.pathout(row,col-1)         
            elif (col+1) <= self.size and (col+1) >= 1 and self.cells_checked[row,col+1] != 1:
                self.pathout(row,col+1)
            else:
                if(len(self.path) - 1) == 0:
                    return 0
                else:
                    del self.path[len(self.path) - 1]
                    a,b = self.path[len(self.path) - 1]
                    self.cells_checked[a,b] = 0
                    del self.path[len(self.path) - 1]
                    self.pathout(a,b)

    def display(self):
        for a in range(0,len(self.path)):
            temp_row, temp_col = self.path[a]
            if self.disp_matrix.has_key((temp_row,temp_col)):
                self.disp_matrix[temp_row,temp_col] = 1

        for row in range(1,(self.size+1)):
            for col in range(1,(self.size+1)):
                print self.disp_matrix[row,col],"\t",
            print "\n"
        

    def show(self):
        print self.path
        print self.matrix
        
if __name__ == "__main__":
    path = []

    print "Enter the number of rows or the number of colums"

    size_matrix = int(raw_input())
    
    maze = Maze(size_matrix)

    # Enter the values of row, col in the matrix to block in the format maze.block(row,col)

    
    # Call maze.random_block() to block random cells in the matrix
    maze.fillup()
    
    if(maze.pathout(size_matrix,1)) == "None":
        print "Path not found \n\n"
    
    maze.display()
    
    
    
    
