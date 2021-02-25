1.
a. (cat /etc/passwd | grep "home" | cut -d":" -f1,4,6 | tail -n +2 | head -n 1 ; cat /etc/passwd | grep "home" | cut -d":" -f1,4,6 | tail -n +4 | head -n 1 ) | sort -n -r -k2 

b. who | grep "jo" | sort -u

