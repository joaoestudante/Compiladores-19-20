################################################################################
############        All rights reserved grupo 54 @ LEIC-A IST       ############
########################## Gonçalo Oliveira - 89449 ############################
########################## José Caniné - 89487 #################################
################################################################################
################################ V1.2 ##########################################
import os
i=0
broken=False
errors = 0
os.system("make clean")
os.system("make")

print("################################################")
print("###############  RUNNING TESTS #################")
print("################################################\n")
for filename in os.listdir("./auto-tests"):
    if filename.endswith(".og"):
        os.system("./og --target xml < auto-tests/"+filename+" > output.txt 2>&1")
        f = open("output.txt")
        line = f.readline()
        if("<sequence_node" not in line):
            print(">>>>> ERROR HERE: "+ filename)
            print(line)
            errors += 1
            broken=True
        else:
            while line:
                if( ("error" in line) or (":" in line) or ("Overflow" in line) or ("*" in line) or ("Unknown" in line) ):
                    print(">>>>> ERROR HERE: {} in line: {} ".format(filename, line))
                    errors += 1
                    broken=True
                    break
                    #print(f.readlines())
                line = f.readline()
        if(not broken):
            print("UGUD: "+filename)
        broken = False
        f.close()
        i += 1
print("################################################")
print("################## RESULTS #####################")
print("################################################")
print(">>>>>>>> " + str(errors) +" failed out of "+ str(i) )

################################################################################
########################## Licensed by DM ######################################
################################################################################
