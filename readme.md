ECE153B
===========

Authors: Maxwell Jung, Baron Young

Labs and projects for Winter 2023 UCSB ECE 153B (Sensor and Peripheral Interface Design).

[src](./src/) contains source codes for each lab/project.


Installation
----------

1. Install Keil using the [set up manual](./lab_manuals/ECE153B_Labs_Getting_Started_W23.pdf)

2. Clone this repository to your home user folder  
   (You can clone to other locations, but syntax highlighting might not work)

    ```console
    git clone https://github.com/MaxwellJung/ECE153B.git
    ```

3. Open the cloned repo on VScode

4. Install [Keil Assistant](https://marketplace.visualstudio.com/items?itemName=CL.keil-assistant) extension on VScode and follow setup instructions (translate the page to English using Google Chrome if needed)

5. Build and Flash the board using the extension

Creating new lab/project
----------------

1. Open Keil IDE by opening the `ece153b_lab.uvprojx` file

2. Right click on the target folder (should be called 'release') displayed inside Keil IDE

3. Click `Add Group...`

4. Rename the newly created folder to the lab/project name

5. Add files from src to the newly created folder

6. Disable all other labs from getting compiled by: a) right clicking on the lab folder, b) clicking `Options for Group '{folder name}'`, and c) unchecking `include in Target Build`