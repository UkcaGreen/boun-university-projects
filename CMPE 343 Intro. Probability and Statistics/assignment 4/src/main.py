import numpy as np

N_POINT = 100000
N_PI    = 100

def apprPi(point_num):
    X = np.random.rand(point_num)
    Y = np.random.rand(point_num)

    in_circle = 0

    for i in range(0,point_num):
        if(X[i]**2 + Y[i]**2 <= 1):
            in_circle = in_circle + 1

    appr_pi = 4*in_circle/point_num

    return appr_pi

pi_array = [apprPi(N_POINT) for i in range(0,N_PI)]

avg_pi = np.average(pi_array)
var_pi = np.var(pi_array)

print(pi_array)
print("Avg: ", avg_pi)
print("Var: ", var_pi)