import os;
import re;
import numpy as np;
i_vals = {
    "10": [],
    "20": [],
    "50": [],
    "100": [],
    "150": [],
    "1000": []
}
for i in i_vals:
    for problem_size in range (1,5):
        for repeat in range (1,20):
            cmndName = "mhe --size" +str(problem_size) +"--iteration  "+ i + "--temp" "2" + \
             "--0 "
            print(cmndName)
            result = os.popen(cmndName)
            output = result.read()
            calcTime = re.findall("dt.*", output)
            if (len(calcTime) > 0):
                calcTime = re.findall("[0-90.]+", calcTime[0])
                result_val = re.findall("[0-90.]+", re.findall("result.*", output)[0])
                i_vals[a].append([problem_size, float(result_val[0]), float(calcTime[0])])
                 

with open("result.plt", "a") as gnuplotfile:
    gnuplotfile.write("set terminal png\n")
    gnuplotfile.write("set output \"result.png\"\n")
    gnuplotfile.write("plot ")
    for i in i_vals:
        print(i)
        summary = i_vals[a]
        #print(summary)
        per_size = {} 
        for values in summary:
            if (per_size.get(values[0]) is None):
                per_size[values[0]] = [[values[1], values[2]]]
            else:
                per_size[values[0]].append([values[1], values[2]])
        #print(per_size)
        for s in per_size:
            combined = np.mean(per_size[s], axis=0)
            with open("result_" + i + ".txt", "a") as myfile:
                myfile.write(str(s) + " " + str(combined[0]) + " " + str(combined[1]) + "\n")
                
        gnuplotfile.write("'result_" + i + ".txt' u 1:2 w lines, ")
    gnuplotfile.write("\n")

result = os.popen("gnuplot result.plt")
output = result.read()            