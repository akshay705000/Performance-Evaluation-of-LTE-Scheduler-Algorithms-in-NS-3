Group assignment 1(LTE Schedulers)     -  	 Group members: Akshay Kumar(cs23mtech11022), Sanket deone(cs23mtech11034)
--------------------------------------------------------------------------------------------------------------------------------------------

1. This assignment consist of plotting 6 different graphs after creating the toplogy as mentioned in the question. 
2. Topolgy is created in the code file named

--------------------------------------------------------------------------------------------------------------------------------------------
Graph 1:

steps:
------

1. Keep the file to rum in the folder scratch of ns3 installation.
2. From terminal go the move to folder "ns-3.40"
3. Run the file named "assignment2_rem.cc" using the command ./ns3 run scratch/assignment2_rem
4. It will generate a file named rems2.out in folder "ns-3.40"
5. Copy the rems2.out to required folder where you want to plot the graph.
6. Also, put the "plot_script.sh" file in the same folder.
7. From terminal go the move to the folder where script and rems2.out file is kept.
8. run the below commands:

chmod +x graph1_plot.sh
./graph1_plot.sh

9. It will generate the plot for graph1.

--------------------------------------------------------------------------------------------------------------------------------------------
Graph 4:

different schedulars used:
--------------------------

PfFfMacScheduler -- Proportional Fair scheduler.
FdMtFfMacScheduler-- Max Throughput (MT) -- frequency division for downlink
RrFfMacScheduler--Round Robin (RR)
FdBetFfMacScheduler--Blind Average Throughput Schedulers (BATS) -- frequency division for downlink

steps:
------

1. Keep the file to rum in the folder scratch of ns3 installation
2. From terminal go the move to folder "ns-3.40"
3. Run the file named "assignment2.cc" using the command given below. PLease move the files named DlRlcStats.txt and DlRsrpSinrStats.txt to other folder where you want to plot graphs.

./ns3 run "scratch/assignment2 --schedType=ns3::PfFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
./ns3 run "scratch/assignment2 --schedType=ns3::FdMtFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
./ns3 run "scratch/assignment2 --schedType=ns3::RrFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
./ns3 run "scratch/assignment2 --schedType=ns3::FdBetFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"

4. Change names of the files copied to another folder with corresponding schedular names after running each of the above commands. 
5. From terminal go the move to the folder where data files are kept.
6. run the given below commans to exact the data for UE 0.

awk '$2==1' DlRsrpSinrStats_fdbet.txt > sinr_fdbet.txt 
awk '$2==1' DlRsrpSinrStats_fdmtff.txt > sinr_fdmtff.txt 
awk '$2==1' DlRsrpSinrStats_pfff.txt > sinr_pfff.txt 
awk '$2==1' DlRsrpSinrStats_rrff.txt > sinr_rrff.txt 

awk '$4==1' DlRlcStats_fdbet.txt > filter_throughput_fdbet.txt 
awk '$4==1' DlRlcStats_fdmtff.txt > filter_throughput_fdmtff.txt 
awk '$4==1' DlRlcStats_pfff.txt > filter_throughput_pfff.txt 
awk '$4==1' DlRlcStats_rrff.txt > filter_throughput_rrff.txt 

awk '{ print $2, $10 * 0.000032}' filter_throughput_fdbet.txt > throughput_fdbet.txt 
awk '{ print $2, $10 * 0.000032}' filter_throughput_fdmtff.txt > throughput_fdmtff.txt 
awk '{ print $2, $10 * 0.000032}' filter_throughput_pfff.txt > throughput_pfff.txt 
awk '{ print $2, $10 * 0.000032}' filter_throughput_rrff.txt > throughput_rrff.txt 

7. Also, put the "graph4_sinr_vs_time.sh" and "graph4_throughput_vs_time.sh" files in the same folder.
8. From terminal go the move to the folder where script and data files are kept.
9. run the below commands:

chmod +x graph4_sinr_vs_time.sh
./graph4_sinr_vs_time.sh
chmod +x graph4_throughput_vs_time.sh
./graph4_throughput_vs_time.sh

10. It will generate the plot graph4_sinr and graph4_throughput.

--------------------------------------------------------------------------------------------------------------------------------------------
Graph 5:

different schedulars used:
--------------------------

PfFfMacScheduler -- Proportional Fair scheduler.
FdMtFfMacScheduler-- Max Throughput (MT) -- frequency division for downlink
RrFfMacScheduler--Round Robin (RR)
FdBetFfMacScheduler--Blind Average Throughput Schedulers (BATS) -- frequency division for downlink

steps:
------

1. Keep the file to rum in the folder scratch of ns3 installation
2. From terminal go the move to folder "ns-3.40"
3. Run the file named "assignment2.cc" using the command given below. PLease move the files named DlRlcStats.txt and DlRsrpSinrStats.txt to other folder where you want to plot graphs.

./ns3 run "scratch/assignment2 --schedType=ns3::PfFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
./ns3 run "scratch/assignment2 --schedType=ns3::FdMtFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
./ns3 run "scratch/assignment2 --schedType=ns3::RrFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
./ns3 run "scratch/assignment2 --schedType=ns3::FdBetFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"

4. Change names of the files copied to another folder with corresponding schedular names after running each of the above commands. 
5. From terminal go the move to the folder where data files are kept.
6. run the given below commans to exact the data for UE 0.

awk '$2==1' DlRsrpSinrStats_fdbet.txt > sinr_fdbet.txt 
awk '$2==1' DlRsrpSinrStats_fdmtff.txt > sinr_fdmtff.txt 
awk '$2==1' DlRsrpSinrStats_pfff.txt > sinr_pfff.txt 
awk '$2==1' DlRsrpSinrStats_rrff.txt > sinr_rrff.txt 

awk '$4==1' DlRlcStats_fdbet.txt > filter_throughput_fdbet.txt 
awk '$4==1' DlRlcStats_fdmtff.txt > filter_throughput_fdmtff.txt 
awk '$4==1' DlRlcStats_pfff.txt > filter_throughput_pfff.txt 
awk '$4==1' DlRlcStats_rrff.txt > filter_throughput_rrff.txt 

awk '{ print $2, $10 * 0.000032}' filter_throughput_fdbet.txt > throughput_fdbet.txt 
awk '{ print $2, $10 * 0.000032}' filter_throughput_fdmtff.txt > throughput_fdmtff.txt 
awk '{ print $2, $10 * 0.000032}' filter_throughput_pfff.txt > throughput_pfff.txt 
awk '{ print $2, $10 * 0.000032}' filter_throughput_rrff.txt > throughput_rrff.txt 

7. Also, put the "graph5_sinr_vs_time.sh" and "graph5_throughput_vs_time.sh" files in the same folder.
8. From terminal go the move to the folder where script and data files are kept.
9. run the below commands:

chmod +x graph5_sinr_vs_time.sh
./graph5_sinr_vs_time.sh
chmod +x graph5_throughput_vs_time.sh
./graph5_throughput_vs_time.sh

10. It will generate the plot graph5_sinr and graph5_throughput.

--------------------------------------------------------------------------------------------------------------------------------------------
Graphs 2, 3, 6:

steps:
------

1. Keep the file to rum in the folder scratch of ns3 installation.
2. Put the "run_simulation_graph236.py" in the folder "ns-3.40".
3. Create a folder named "output234" in the folder "ns-3.40".
4. From terminal go the move to folder "ns-3.40".
5. Run the python script file using the given below command 

run_simulation_graph236.py

6. It will take some time to generate outputs that will be generated in the folder"output234" inside the folder "ns-3.40".
7. Copy the folder somewhere where you to analyse the data. 

For graph3
----------

1. Sort the above generated data in different folder and subfolder based on schedular type and half,full buffer.
2. run script throughput_processing_graph_3.py in all the subfolder.
3. select differnt 4 file for each half and full buffer of different schedular with different seed.
4. Run the script graph3_cumulative_probability_script.py
5. Run script graph3_plot_0.sh and graph3_plot_5.sh to generate plots.

Graphs 2, 6:
------------

1. After step 7 of common steps for 2,3,6 run the script throughput_script_26.py on all the data.
2. Run script avergare_the_26.py on data generated on above step which will generate 2 files used for poltting.
3. Run the script graphs2.sh and graphs6.sh on above generated two files for respective outputs.


9.
