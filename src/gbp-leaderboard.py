"""
Print your own GBP leaderboard!
"""
import requests

#fetch latest gbp
response = requests.get(url = "https://raw.githubusercontent.com/tgstation/tgstation/gbp-balances/.github/gbp-balances.toml")
content = response.text

initial_file = []
line = ""
for char in content: # collect web page into each line
    line += char
    if char == '\n':
        if (ord(line[0]) >= ord('0') and ord(line[0]) <= ord('9')): #make sure it starts with a github ID, else its not a valid line
            initial_file.append(line)
        line = ""

gbp = []
for line in initial_file: # split this into username and GBP
    segments = line.split(" ")
    temp = segments[-1]
    if temp[-1:] == '\n':
        segments[-1] = temp[:-1]
    gbp.append([segments[-1], int(segments[2])])

for n in range(len(gbp)-1, 0, -1): #bubble sort
    for i in range(n):
        if gbp[i][1] < gbp[i + 1][1]:
            gbp[i][1], gbp[i+1][1] = gbp[i+1][1], gbp[i][1]
            gbp[i][0], gbp[i+1][0] = gbp[i+1][0], gbp[i][0]

with open("balances.txt", 'w') as file: # write to file
    i = 1 
    for item in gbp:
        file.write("#" + str(i) + " " + item[0] + " " + str(item[1]) + '\n')
        i += 1
