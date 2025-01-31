def main():
   print("starting eic2_clean.py")
   # Check if the script is already running
   numprocesses_running=subprocess.check_output(["echo `ps all -u tbritton | grep eic2_clean.py | grep -v grep | wc -l`"], shell=True)
   print(f"numprocesses_running: {numprocesses_running}")
   if(int(numprocesses_running) >=2 ):
       print("Script already running. Exiting...")
       sys.exit(0)
   # Define the command to run


   subdir_arr = ['22.10.0/', '22.11.0/', '22.11.2/', '22.11.3/', '22.12.0/','23.01.0/', '23.03.0/', '23.05.1/', '23.05.2/', '23.06.0/', '23.06.1/', '23.07.1/', '23.07.2/', '23.08.0/', '23.09.0/', '23.09.1/', '23.10.0/', '23.11.0/', '23.12.0/']
   #command_str = 'jmirror -limit 5000 -min-age 31536000 -delete written /work/eic2/EPIC/FULL/ /work/eic2/ /mss/eic/'
   #command_str2 = 'jmirror -limit 5000 -min-age 31536000 -delete written /work/eic2/EPIC/RECO/22.11.2 /work/eic2/ /mss/eic/'


   #command_str3 = 'jmirror -limit 5000 -min-age 31536000 -delete written /work/eic2/EPIC/RECO/22.11.3 /work/eic2/ /mss/eic/'



   for subdir in subdir_arr:
       command_str = 'jmirror -limit 15000 -min-age 31536000 -delete written /work/eic2/EPIC/RECO/' + subdir + ' /work/eic2/ /mss/eic/'
       print("beginning loop of", command_str)
       try:
           # Run the command and get standard output
           output=subprocess.run(command_str.split(" "), check=True, env=os.environ)



       except subprocess.CalledProcessError as e:
           print(f"Error running command: {e}")


       
       try:
           # Run the command
           print("beginning 2nd loop of", command_str)
           output2=subprocess.run(command_str.split(" "), capture_output=True,check=True,text=True)


       except subprocess.CalledProcessError as e:
           print(f"Error running command: {e}")


   
       if "Found 0 files, comprising 0 bytes" in output2.stdout:
           print("No files found. Continuing...")
           continue
       else:
           return