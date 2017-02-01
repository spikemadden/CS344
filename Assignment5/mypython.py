import sys
import random
import string

def make_file():

		
	for x in range (0, 3):	
		print "\ncreating file number", x + 1
		string = generate_string()
		print "the content of the file is:", string
		file = open(str(x + 1), 'w');
		file.write(string)
		file.close()


def generate_string():

	word = random.choice(string.ascii_lowercase)
	for x in range (0, 9):
		word = word + random.choice(string.ascii_lowercase)
	
	return word



def mult():

	num1 = random.randint(1,42)
	num2 = random.randint(1,43)
	
	print "\nThe first number is:", num1
	print "The second number is:", num2
	print "The product is:", num1 * num2


make_file()
mult();

