# Emilcan Arıcan
# 2016400231
# Compiling
# Working

from mpi4py import MPI
import numpy as np
import math
import sys

comm = MPI.COMM_WORLD

T           = int(sys.argv[3])
rank        = comm.Get_rank()
max_rank    = comm.size - 1

#number of chunks along one edge of the map
period      = int(math.sqrt(max_rank))

#length of the one side of a chunk
chunk_edge = int(360//period)

#functions that calculate the ranks of the neighbor processes
def upChunk(r):
    if r <= period:
        return r - period + max_rank
    else:
        return r - period

def downChunk(r):
    if (r > (max_rank - period)):
        return r + period - max_rank
    else:
        return r + period

def leftChunk(r):
    if r % period == 1:
        return r - 1 + period
    else:
        return r - 1

def rightChunk(r):
    if r % period == 0:
        return r + 1 - period
    else:
        return r + 1

def upLeftChunk(r):
    return leftChunk(upChunk(r))

def upRightChunk(r):
    return rightChunk(upChunk(r))

def downLeftChunk(r):
    return leftChunk(downChunk(r))

def downRightChunk(r):
    return rightChunk(downChunk(r))

# functions that calculate where chunks should be started and be ended at
def widthBegin(r):
    return chunk_edge*((r-1)%period)

def widthEnd(r):
    return chunk_edge*(((r-1)%period)+1)

def heightBegin(r):
    return chunk_edge*(((r-1)//period))
    
def heightEnd(r):
    return chunk_edge*(((r-1)//period)+1)

#function that checks if it is a white ranked process
def isWhite():
    return (((rank-1)%period)+((rank-1)//period))%2 == 1

#function that checks if it is a even ranked process
def isEven():
    return rank%2 == 0

#function that determines the next state of a location
#depending on current state and the number of neighbors
def nextState(currentState, neighborNum):
    if currentState == 1:
        if neighborNum < 2:
            return 0
        elif neighborNum > 3:
            return 0
        else:
            return 1
    elif currentState == 0:
        if neighborNum == 3:
            return 1
        else:
            return 0

#calculates the number of the neighbors
def neighborNum(_map, i, j):
    return (_map[i-1][j-1] 
            + _map[i-1][j] 
            + _map[i][j-1]
            + _map[i+1][j]
            + _map[i][j+1] 
            + _map[i+1][j+1]
            + _map[i-1][j+1]
            + _map[i+1][j-1])

def main():
    #manager
    if rank == 0:
        
        #input-outputfiles
        inputfile   = sys.argv[1]
        outputfile  = sys.argv[2]

        #read input from file to numpy array
        map = np.loadtxt(inputfile, dtype=int)

        #split map into chunks; send chunks to workers 
        for r in range(1,max_rank+1):

            data = map[heightBegin(r):heightEnd(r) , widthBegin(r):widthEnd(r)].copy()

            comm.Send(data, dest=r, tag=0)

        #variable that holds concatenated chunks
        chunks = np.empty(shape=[chunk_edge,chunk_edge*period], dtype=int)

        #receive chunks from workers and concatenate them
        for i in range(0,period):

            #temporary variable that holds concatenated chunks
            temp = np.empty(shape=[chunk_edge,chunk_edge], dtype=int)

            for j in range(0,period):

                #receive data from worker
                data = np.empty(shape=[chunk_edge,chunk_edge], dtype=int)
                comm.Recv(data, source=(period*i+j+1))

                #concatenate
                if j == 0:
                    temp = data
                else:
                    temp = np.concatenate((temp,data),axis=1)

            #concatenate
            if i == 0:
                chunks = temp
            else:
                chunks = np.concatenate((chunks,temp))

        #save into file
        np.savetxt(outputfile, chunks, delimiter=" ", fmt='%i', newline=" \n")

    #worker
    if rank > 0:

        #receive data from the manager
        data = np.empty(shape=[chunk_edge,chunk_edge], dtype=int)
        comm.Recv(data, source=0, tag=0)
        
        #send horizontal and vertical function
        def sendHorizontalAndVertical(tag1, tag2, tag3, tag4):
            comm.Send(upRow_s,      dest=upChunk(rank), tag=tag1)
            comm.Send(downRow_s,    dest=downChunk(rank), tag=tag2)
            comm.Send(leftCol_s,    dest=leftChunk(rank), tag=tag3)
            comm.Send(rightCol_s,   dest=rightChunk(rank), tag=tag4)

        #receive horizontal and vertical function
        def recvHorizontalAndVertical(tag1, tag2, tag3, tag4):
            comm.Recv(downRow_r,    source=downChunk(rank), tag=tag1)
            comm.Recv(upRow_r,      source=upChunk(rank), tag=tag2)
            comm.Recv(rightCol_r,   source=rightChunk(rank), tag=tag3)
            comm.Recv(leftCol_r,    source=leftChunk(rank), tag=tag4)
        
        #send diagonal function
        def sendDiagonal(tag1, tag2, tag3, tag4):
            comm.Send(upRight_s,    dest=upRightChunk(rank), tag=tag1)
            comm.Send(upLeft_s,     dest=upLeftChunk(rank), tag=tag2)
            comm.Send(downRight_s,  dest=downRightChunk(rank), tag=tag3)
            comm.Send(downLeft_s,   dest=downLeftChunk(rank), tag=tag4)
        
        #receive diagonal function
        def recvDiagonal(tag1, tag2, tag3, tag4):
            comm.Recv(downLeft_r,   source=downLeftChunk(rank), tag=tag1)
            comm.Recv(downRight_r,  source=downRightChunk(rank), tag=tag2)
            comm.Recv(upLeft_r,     source=upLeftChunk(rank), tag=tag3)
            comm.Recv(upRight_r,    source=upRightChunk(rank), tag=tag4)

        for _ in range(0,T):

            #variables that will be sent
            upRow_s     = data[0,:].copy()
            downRow_s   = data[-1,:].copy()
            leftCol_s   = data[:,0].copy()
            rightCol_s  = data[:,-1].copy()
            upLeft_s    = data[0,0].copy()
            upRight_s   = data[0,-1].copy()
            downLeft_s  = data[-1,0].copy()
            downRight_s = data[-1,-1].copy()
            
            #variables that will be received
            upRow_r     = np.empty(chunk_edge, dtype=int)
            downRow_r   = np.empty(chunk_edge, dtype=int)
            leftCol_r   = np.empty(shape=[1,chunk_edge], dtype=int)
            rightCol_r  = np.empty(shape=[1,chunk_edge], dtype=int)
            upLeft_r    = np.empty(1, dtype=int)
            upRight_r   = np.empty(1, dtype=int)
            downLeft_r  = np.empty(1, dtype=int)
            downRight_r = np.empty(1, dtype=int)

            #white tiles (like a chess board)
            if isWhite():
                sendHorizontalAndVertical(1,2,3,4)
                recvHorizontalAndVertical(5,6,7,8)

            #black tiles (like a chess board)
            else:
                recvHorizontalAndVertical(1,2,3,4)
                sendHorizontalAndVertical(5,6,7,8)
            
            #even ranked tiles
            if isEven():
                sendDiagonal(9,10,11,12)
                recvDiagonal(13,14,15,16)

            #odd ranked tiles
            else:
                recvDiagonal(9,10,11,12)
                sendDiagonal(13,14,15,16)

            #concatenate parts that collected from neighbors with data
            top_row = np.concatenate((upLeft_r,upRow_r,upRight_r))
            mid_rows = np.concatenate((np.transpose(leftCol_r),data,np.transpose(rightCol_r)), axis=1)
            bot_row = np.concatenate((downLeft_r,downRow_r,downRight_r))
            framed_data = np.concatenate((np.transpose(top_row[:,None]),mid_rows,np.transpose(bot_row[:,None])))

            #create next_data that next state will be calculated in
            next_data = np.empty(shape=[chunk_edge,chunk_edge], dtype=int)

            for i in range(1,chunk_edge+1):
                for j in range(1,chunk_edge+1):

                    #number of neighbors
                    neighbor_num = neighborNum(framed_data, i, j)
                    
                    #fill the next_data array
                    next_data[i-1][j-1] = nextState(framed_data[i][j], neighbor_num)

            #assign next_data to data for the next iteration
            data = next_data.copy()

        #send the final result to manager
        comm.Send(data, dest=0, tag=17)

main()