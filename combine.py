# read from the five directories and combine the data into one file
# output: one file with all the data
# input: five directories

import os

vary_params = {
    'plot1': {'param': 'blocksize', 'values': [8, 16, 32, 64, 128]},
    'plot2': {'param': 'L1_size', 'values': [512, 1024, 2048, 4096, 8192]},
    'plot3': {'param': 'L1_assoc', 'values': [1, 2, 4, 8, 16]},
    'plot4': {'param': 'L2_size', 'values': [16384, 32768, 65536, 131072, 262144]},
    'plot5': {'param': 'L2_assoc', 'values': [1, 2, 4, 8, 16]}
}

file_patterns = {
    'plot1': 'trace{}_blocksize_{}.txt',
    'plot2': 'trace{}_L1_size_{}.txt',
    'plot3': 'trace{}_L1_assoc_{}.txt',
    'plot4': 'trace{}_L2_size_{}.txt',
    'plot5': 'trace{}_L2_assoc_{}.txt'
}

L1=[[[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []]]
L2=[[[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []]]
L3=[[[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []]]
L4=[[[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []]]
L5=[[[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []], [[], [], [], [], []]]

with open('graph_data.txt', 'w') as output_file:
    ## Data for plots
    for plot in vary_params:
        for value in vary_params[plot]['values']:
            for i in range(1, 9):
                file_name = file_patterns[plot].format(i, value)
                file_path = os.path.join(plot, file_name)
                # print(file_path)
                with open(file_path, 'r') as f:
                    content = f.read().split('\n')
                    l1_reads = int(content[3].split(':')[1])
                    l1_writes = int(content[5].split(':')[1])
                    l1_writemiss = int(content[6].split(':')[1])
                    l1_readmiss = int(content[4].split(':')[1])
                    l1_writeback = int(content[8].split(':')[1])
                    l2_readmiss = int(content[10].split(':')[1])
                    l2_writemiss = int(content[12].split(':')[1])
                    l2_writeback = int(content[14].split(':')[1])
                    time = ( l1_reads + l1_writes )* 1 + (l1_writemiss + l1_readmiss + l1_writeback)*20 + (l2_writemiss + l2_readmiss + l2_writeback)*200
                    if plot == 'plot1':
                        if value == 8:
                            L1[i-1][0] = time
                        elif value == 16:
                            L1[i-1][1] = time
                        elif value == 32:
                            L1[i-1][2] = time
                        elif value == 64:
                            L1[i-1][3] = time
                        elif value == 128:
                            L1[i-1][4] = time
                    elif plot == 'plot2':
                        if value == 512:
                            L2[i-1][0] = time
                        elif value == 1024:
                            L2[i-1][1] = time
                        elif value == 2048:
                            L2[i-1][2] = time
                        elif value == 4096:
                            L2[i-1][3] = time
                        elif value == 8192:
                            L2[i-1][4] = time
                    elif plot == 'plot3':
                        if value == 1:
                            L3[i-1][0] = time
                        elif value == 2:
                            L3[i-1][1] = time
                        elif value == 4:
                            L3[i-1][2] = time
                        elif value == 8:
                            L3[i-1][3] = time
                        elif value == 16:
                            L3[i-1][4] = time
                    elif plot == 'plot4':
                        if value == 16384:
                            L4[i-1][0] = time
                        elif value == 32768:
                            L4[i-1][1] = time
                        elif value == 65536:
                            L4[i-1][2] = time
                        elif value == 131072:
                            L4[i-1][3] = time
                        elif value ==  262144:
                            L4[i-1][4] = time
                    elif plot == 'plot5':
                        if value == 1:
                            L5[i-1][0] = time
                        elif value == 2:
                            L5[i-1][1] = time
                        elif value == 4:
                            L5[i-1][2] = time
                        elif value == 8:
                            L5[i-1][3] = time
                        elif value == 16:
                            L5[i-1][4] = time
                    # print(L1)
    # print(L1)
    ### For plot 1 
    output_file.write('PLOT 1 :')
    output_file.write('\n')
    output_file.write('TRACE NUMBER :           8           16          32          64          128')
    output_file.write('\n')
    for i in range(0, 8):
        output_file.write('TRACE {} :\t\t\t\t'.format(i+1))
        for j in range(0, 5):
            output_file.write('{}\t\t\t'.format(L1[i][j]))
        output_file.write('\n')
    ### For plot 2
    output_file.write('PLOT 2 :')
    output_file.write('\n')
    output_file.write('TRACE NUMBER :           512         1024        2048        4096        8192')
    output_file.write('\n')
    for i in range(0, 8):
        output_file.write('TRACE {} :\t\t\t\t'.format(i+1))
        for j in range(0, 5):
            output_file.write('{}\t\t\t'.format(L2[i][j]))
        output_file.write('\n')
    ### For plot 3
    output_file.write('PLOT 3 :')
    output_file.write('\n')
    output_file.write('TRACE NUMBER :           1           2           4           8           16')
    output_file.write('\n')
    for i in range(0, 8):
        output_file.write('TRACE {} :\t\t\t\t'.format(i+1))
        for j in range(0, 5):
            output_file.write('{}\t\t\t'.format(L3[i][j]))
        output_file.write('\n')
    ### For plot 4
    output_file.write('PLOT 4 :')
    output_file.write('\n')
    output_file.write('TRACE NUMBER :           16384       32768       65536       131072      262144')
    output_file.write('\n')
    for i in range(0, 8):
        output_file.write('TRACE {} :\t\t\t\t'.format(i+1))
        for j in range(0, 5):
            output_file.write('{}\t\t'.format(L4[i][j]))
        output_file.write('\n')
    ### For plot 5
    output_file.write('PLOT 5 :')
    output_file.write('\n')
    output_file.write('TRACE NUMBER :           1           2           4           8           16')
    output_file.write('\n')
    for i in range(0, 8):
        output_file.write('TRACE {} :\t\t\t\t'.format(i+1))
        for j in range(0, 5):
            output_file.write('{}\t\t\t'.format(L5[i][j]))
        output_file.write('\n')
    output_file.close()
