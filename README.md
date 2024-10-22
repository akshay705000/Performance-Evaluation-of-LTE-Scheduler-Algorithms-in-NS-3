
# Group Assignment 1: LTE Schedulers

### Group Members:
- **Akshay Kumar** (cs23mtech11022)
- **Sanket Deone** (cs23mtech11034)

This project involves plotting six different graphs based on the LTE topology using various scheduling algorithms. The implementation is done using the ns-3 network simulator, and the steps to generate each graph are described in detail below.

## Graph 1: Resource Element Map (REM)

### Steps:
1. Place the code file `assignment2_rem.cc` in the `scratch` folder of your ns3 installation.
2. Navigate to the `ns-3.40` directory in the terminal:
   ```bash
   cd ns-3.40
   ```
3. Run the file:
   ```bash
   ./ns3 run scratch/assignment2_rem
   ```
4. This generates a file named `rems2.out` in the `ns-3.40` directory.
5. Move the `rems2.out` file to the folder where you want to plot the graph.
6. Place the `graph1_plot.sh` script in the same folder.
7. In the terminal, navigate to the folder where both the script and `rems2.out` are located:
   ```bash
   cd path/to/your/folder
   ```
8. Run the commands to generate the graph:
   ```bash
   chmod +x graph1_plot.sh
   ./graph1_plot.sh
   ```
9. This will generate the plot for **Graph 1**.

---

## Graph 4: Scheduler Comparison

### Schedulers Used:
- **PfFfMacScheduler**: Proportional Fair scheduler
- **FdMtFfMacScheduler**: Max Throughput scheduler (MT)
- **RrFfMacScheduler**: Round Robin scheduler (RR)
- **FdBetFfMacScheduler**: Blind Average Throughput Schedulers (BATS)

### Steps:
1. Place the code file `assignment2.cc` in the `scratch` folder of ns3.
2. Navigate to the `ns-3.40` folder:
   ```bash
   cd ns-3.40
   ```
3. Run the file with each scheduler using the following commands:
   ```bash
   ./ns3 run "scratch/assignment2 --schedType=ns3::PfFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
   ./ns3 run "scratch/assignment2 --schedType=ns3::FdMtFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
   ./ns3 run "scratch/assignment2 --schedType=ns3::RrFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
   ./ns3 run "scratch/assignment2 --schedType=ns3::FdBetFfMacScheduler --speed=0.0 --RngRun1=22 --udpFullBuffer=true"
   ```
4. Move the generated files (`DlRlcStats.txt` and `DlRsrpSinrStats.txt`) to a separate folder and rename them according to the scheduler.
5. Navigate to the folder where the files are stored and filter data for **UE 0** using these commands:
   ```bash
   awk '$2==1' DlRsrpSinrStats_schedulername.txt > sinr_schedulername.txt
   awk '$4==1' DlRlcStats_schedulername.txt > filter_throughput_schedulername.txt
   awk '{ print $2, $10 * 0.000032 }' filter_throughput_schedulername.txt > throughput_schedulername.txt
   ```
6. Place the plotting scripts (`graph4_sinr_vs_time.sh` and `graph4_throughput_vs_time.sh`) in the same folder.
7. Run the commands to generate the plots:
   ```bash
   chmod +x graph4_sinr_vs_time.sh
   ./graph4_sinr_vs_time.sh
   chmod +x graph4_throughput_vs_time.sh
   ./graph4_throughput_vs_time.sh
   ```
8. This will generate the plots for **Graph 4 (SINR and Throughput)**.

---

## Graph 5: Scheduler Throughput

Follow the same steps as Graph 4 to run the simulation, generate the data files, and plot **Graph 5** for throughput comparison among schedulers.

---

## Graphs 2, 3, and 6:

### Steps:
1. Place the Python script `run_simulation_graph236.py` in the `ns-3.40` directory.
2. Create a folder named `output234` in the `ns-3.40` directory.
3. Navigate to the `ns-3.40` directory:
   ```bash
   cd ns-3.40
   ```
4. Run the simulation script:
   ```bash
   ./run_simulation_graph236.py
   ```
5. The outputs will be generated in the `output234` folder. Move the folder to another location for further analysis.
6. For **Graph 3**, sort the generated data by scheduler type and run the script `throughput_processing_graph_3.py` in each folder.
7. Run the plotting scripts `graph3_cumulative_probability_script.py`, `graph3_plot_0.sh`, and `graph3_plot_5.sh` to generate the plots for Graph 3.
8. For **Graphs 2 and 6**, run the scripts `throughput_script_26.py` and `avergae_the_26.py` on the data, then use `graphs2.sh` and `graphs6.sh` to generate the respective plots.
