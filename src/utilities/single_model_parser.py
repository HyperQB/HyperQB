import sys
import pynusmv
import string
import re
import os.path
from os import path
from pynusmv.model 			import *
from pynusmv.fsm 			import BddTrans
from pynusmv.fsm 			import BddEnc
from pynusmv.fsm 			import BddFsm
from pynusmv.fsm 			import SymbTable
from pynusmv 				import prop
from pynusmv.mc 			import check_ctl_spec
from pynusmv.mc 			import check_ltl_spec
from pynusmv.bmc 			import *
from pynusmv 				import node
from pynusmv 				import glob
from pynusmv.glob 			import *
from pynusmv.bmc 			import glob as bmcGlob
from pynusmv.node          	import *
from pynusmv.parser			import parse_ltl_spec
from pynusmv.bmc.utils      import DumpType , BmcModel
from pynusmv.be.expression  import Be
from pynusmv.bmc.glob       import go_bmc, bmc_exit, master_be_fsm, BmcSupport
from pynusmv.bmc.invarspec  import *
from pynusmv.utils          import StdioFile
from pynusmv.wff 			import Wff
from pynusmv.be.encoder 	import BeEnc
from pynusmv.be.fsm        	import BeFsm
from pynusmv.prop 			import PropDb
from pynusmv.bmc           	import utils as bmcutils
from pynusmv.be.manager		import BeManager
from pynusmv.sexp.fsm  		import *



#########################
#  little helpe methods #
#########################


## Logical operators ##
NOT = "~"
AND = "/\\"
OR = "\\/"
IMPLIES = " -> "
IFF = "<->"
NEXT = "'"



#################
#  remove stuff #
#################

## TBD: can we have the name includes dot in genQBF? ##
def remove_dot(s):
	s =  s.replace(".", "-")
	return s
########################################################



############################
#  Check type of variables #
############################

## value is boolean ##
def isBoolean(key):
	return ((key == 'TRUE') or (key == 'FALSE'))

## if values is digits ##
def isNumber(key):
	return key.isdigit()


###################################
#  Initial Condition Construction #
###################################

## assign a boolean value to a variable key ##
def assignBool(key, dict):
	var_name = remove_dot(key)
	if (dict[key] == 'TRUE'):
		return var_name
	else:
		return NOT+var_name

## assign a digit value to a variable key ##
def assignBin(key, dict):
	bits = []
	var_name = remove_dot(key)

	num_bits = bitblasting_dict[key]
	binary = format(int(dict[key]), '0'+str(num_bits)+'b').replace("0b", "")
	counter = num_bits-1
	for b in binary:
		if (b == '1'):
			bits.append(var_name+"_"+str(counter))
		else:
			bits.append(NOT+var_name+"_"+str(counter))
		counter-=1
	return conjunct(bits)

## assign a bool variable value to a variable key ##
def assignVal(key, dict):
	var_name = remove_dot(key)
	return var_name+IFF+dict[key]



#####################################
#  Transition Relation Construction #
#####################################


# print out all atomic propositions in dict
## All AP values in the current state ##
def getDictAP(dict):
	all_ap = []
	for ap in dict:
		value = dict[ap]
		if(isBoolean(value)):
			all_ap.append(assignBool(ap, dict))
		elif(isNumber(value)):
			all_ap.append(assignBin(ap, dict))
		else:
			name = remove_dot(ap)
			all_ap.append(name+IFF+dict[ap])
	return conjunct(all_ap)

## All AP values in the post state ##
def getNextDictAP(dict):
	all_ap = []
	for ap in dict:
		value = dict[ap]
		if(isBoolean(value)):
			all_ap.append(assignNextBool(ap, dict))
		elif(isNumber(value)):
			all_ap.append(assignNextBin(ap, dict))
		else:
			all_ap.append(assignNextVal(ap, dict))
	return conjunct(all_ap)

## All AP values in the post state ##
def assignNextBin(key, dict):
	bits = []
	name = remove_dot(key)
	num_bits = bitblasting_dict[key]
	binary = format(int(dict[key]),'0'+str(num_bits)+'b').replace("0b", "")
	counter = num_bits-1
	for b in binary:
		if (b == '1'):
			bits.append(name+"_"+str(counter)+NEXT)
		else:
			bits.append(NOT+name+"_"+str(counter)+NEXT)
		counter-=1
	return conjunct(bits)

def assignNextBool(key, dict):
	var_name = remove_dot(key)
	if (dict[key] == 'TRUE'):
		return var_name+NEXT
	else:
		return NOT+var_name+NEXT

def assignNextVal(key, dict):
	var_name = remove_dot(key)
	return var_name+NEXT+IFF+dict[key]+NEXT


####################################
#  Logical Expression Construction #
####################################


## conjunct of all element in a list ##
def conjunct(list):
	if(len(list) == 1):
		return list[0]
	output = ""
	for i in range(len(list)-1):
		output += list[i] + AND
	return "(" + output + list[len(list)-1] + ")"

## disjunct of all element in a list ##
def disjunct(list):
	if(len(list) == 1):
		return list[0]
	output = ""
	for i in range(len(list)-1):
		output += list[i] + OR
	return "(" + output + list[len(list)-1] + ")"


def conjunct_trans(list):
	output = ""
	for i in range(len(list)-1):
		output += list[i] + AND + "\n"
	return output + list[len(list)-1]

def trans(pre, post):
	return "(("+pre+")"+IMPLIES+"("+post+"))"



###############################
#  HLTL Property Construction #
###############################


### TODO
def model_sus(str, sus):
	# str = re.sub('\d', '\d', str)
	# for ele in str:
	#     if ele.isdigit():
	#         str = str.replace(ele, ele+sus)
	return str

# bin_eq: two variables matches each bit
def binary_eq(var_l, var_r):
	num_bits = bitblasting_dict[var_l[0]]
	result = []
	for i in range(num_bits):
		left = var_l[0] + "_"+str(i) +"_"+var_l[1]
		right = var_r[0] + "_"+str(i) +"_"+var_r[1]
		# print(left + " <-> " + right)
		result.append("("+left + IFF + right+")")
	return conjunct(result)

def binary_assign(var, num):
	num_bits = bitblasting_dict[var[0]]
	if (num > (2**(num_bits))):
		print("[ !!! HyperQube error: variable ", var[0], " is assigned a value that is out of the bound of its definition in the NuSMV model." )
		quit()
	binary = format(num, '0'+str(num_bits)+'b').replace("0b", "")
	result = []
	counter = num_bits-1
	for b in binary:
		if (b == '1'):
			result.append(var[0] + "_"+str(counter) +"_"+var[1])
		else:
			result.append(NOT+var[0] + "_"+str(counter) +"_"+var[1])
		counter-=1
	return conjunct(result)




###############
# (Not using) #
###############
def remove_spaces(s):
	return s.replace("\n","")

def format_str(s):
	return ((s.replace(".", "_")).replace("!","~")).replace("&","/\\").replace("|","\/")

def format_next_str(s):
	return "(" + format_str(s).replace("next","").replace("(","").replace(")","") + "')"

def format_post(post):
	post_lst = post.split()
	for i in range(len(post_lst)):
		if("next" in post_lst[i]):
			post_lst[i] = format_next_str(post_lst[i])
	return format_str(' '.join(str(x) for x in post_lst))

def format_trans(tr):
	if (tr is ''):
		return ""
	if (":" not in tr):
		return format_next_str(tr)
	tr = tr.split(":")
	pre = format_str(tr[0])
	post = format_post(tr[1])
	return trans(pre, post)



#########################
#      Get Arguments    #
#########################
smv_file_name = sys.argv[1]
fomula_file_name = sys.argv[2]
parsed_model_file_I_name = sys.argv[3]
parsed_model_file_R_name = sys.argv[4]
translated_formula_file_name = sys.argv[5]
QS_file_name = sys.argv[6]

FLAG = ""
if(len(sys.argv)==8):
	FLAG = sys.argv[7]

To_Negate_formula=(FLAG=="" or FLAG=="-bughunt")



print("\n=== Read SMV Model ===")
#########################
#  Model Initialization #
#########################
pynusmv.init.init_nusmv()
pynusmv.glob.load_from_file(smv_file_name)
pynusmv.glob.compute_model()
fsm = pynusmv.glob.prop_database().master.bddFsm
enc = fsm.bddEnc

print("\n[ success! SMV model accepted. ]")

# ### DEBUG
# print("=========")
# print("FSM Model info:")
print("\n=== Parse SMV Model ===")
state_variables = list(enc.stateVars)

# print("All variables: ")
# print("state variables: ", state_variables)
defined_variables = list(enc.definedVars)
# print("defined variables: ", defined_variables)

valid_variables = state_variables + defined_variables
# print(valid_varaibles)


num_states = fsm.count_states(fsm.reachable_states)
print("Total number of reachable states: ", num_states)
# inputs = list(enc.inputsVars)
# print("input variables", inputs)
# atomics = list(enc.definedVars)
# print("atomic propositions", atomics)





############################
#  bit-blasting dictionary #
############################
bitblasting_dict = {}
# build bitblasting_dict
smv_file = open(smv_file_name, 'r')
lines = smv_file.readlines()
for line in lines:
	# print(line)
	if(re.findall("\.\.", line)):
		line = line.split("--", 1)[0].replace("\t","") #remove comments

		if(not line.replace(" ","")):
			pass
		# print(line)
		elif (line): # if it's not empty
			key = re.findall(".*:", line)[0].replace(":","")
			num = re.findall("[\d]*;", line)[0].replace(";","")
			# print(line)
			# print(key)
			# print(num)
			value = int(num).bit_length()

			# bitblasting_dict[key] = value
			for var in state_variables:
				if(key.replace(" ", "") in var):
					bitblasting_dict[var] = value
	# TODO: new data type: unsigned word
	elif(re.findall("unsigned word", line)):
		line = line.split("--", 1)[0].replace("\t","") #remove comments
		# print(line)
		if (line): # if it's not empty
			key = re.split("unsigned", line)[0].replace(":","")
			num_bits = re.split("word", line)[1].replace("[","").replace("]","")
			# print(key)
			# print(num_bits)
			for var in state_variables:
				if(key.replace(" ", "") in var):
					bitblasting_dict[var] = int(num_bits.replace("\n","").replace(";",""))


# print(value)
# print(int(value).bit_length())

print("Dictionary for bit-blasting variables (number of bits needed for each): ")
print(bitblasting_dict)



##################################
#  unavailable varaible checking #
##################################
def check_var(name):
	for var in state_variables:
		if(var == name):
			return true

	return false







###################################
#  Initial Condition Construction #
###################################
def gen_I():
	init_states = []
	for state in fsm.pick_all_states(fsm.init):
		init_conditions = []
		dictionary = state.get_str_values()
		for ap in dictionary:
			value = dictionary[ap]
			# print(value)
			if(isBoolean(value)):
				init_conditions.append(assignBool(ap, dictionary))
			elif(isNumber(value)):
				init_conditions.append(assignBin(ap, dictionary))
			# TODO: new data type: unsigned word
			elif("0ud" in value):
				dictionary[ap] = re.split("_", value)[1]
				init_conditions.append(assignBin(ap, dictionary))
			else:
				init_conditions.append(assignVal(ap, dictionary))
		init_states.append(conjunct(init_conditions))
	##  write to I_bool file
	I_bool = open(parsed_model_file_I_name, "w")
	I_bool.write(disjunct(init_states))
	I_bool.close()


#####################################
#  Transition Relation Construction #
#####################################
def gen_R():
	all_transitions = []
	R_bool = open(parsed_model_file_R_name, "w")
	for state in fsm.pick_all_states(fsm.reachable_states):
		transitions = []
		curr = state.get_str_values()
		# print('from')
		# print(curr)
		# print('to')
		post_list = [] # list of all next states
		for post_state in fsm.pick_all_states(fsm.post(state)):
			post = post_state.get_str_values()
			# print(post)
			post_list.append(getNextDictAP(post))
		transitions.append(trans(getDictAP(curr), disjunct(post_list)))
		R_bool.write(trans(getDictAP(curr), disjunct(post_list)))
		# R_bool.write(conjunct_trans(transitions))
		R_bool.write(AND + "\n")

	##  write to R_bool file
	# R_bool = open("test_R.bool", "w")
	R_bool.write("TRUE"); ##DUMMY
	# R_bool.write(conjunct_trans(all_transitions))
	R_bool.close()

	## DEBUG
	# print("\n--------------------")
	# print("transition relations")
	# print("--------------------")
	# print(conjunct_trans(all_transitions))

### for DEBUG
# print("initial state")
# print(fsm.count_states(fsm.init))
# for state in fsm.pick_all_states(fsm.init):
#     print(state.get_str_values())
#
# print("all reachable state")
# print(fsm.count_states(fsm.reachable_states))
# for state in fsm.pick_all_states(fsm.reachable_states):
#     print(state.get_str_values())
#
# print("pre-post condition")
# for state in fsm.pick_all_states(fsm.reachable_states):
# 	print("post conditions of ", state.get_str_values() )
# 	for post_cond in fsm.pick_all_states(fsm.post(state)):
# 		print(post_cond.get_str_values())


# generating files for genQBF
gen_I()
gen_R()
print("\n[ success! SMV model parsed into Boolean Expressions. ]")
# print("\n[ success! SMV model parsed into Boolean Expressions. " + parsed_model_file_I_name + parsed_model_file_R_name+"]")

##################################
#  HyperLTL Formula Construction #
##################################
print("\n=== Translate HyperLTL Formula ===")
text = ""
file = open(fomula_file_name, 'r')
Lines = file.readlines()
for line in Lines:
	if ("#" not in line):
		text += line

# print("user input formula: \n"+text)



## detect the optional flag
if (FLAG == "-bughunt"):
	print("(detect -bughunt flag, formula negated.)\n")
elif  (FLAG == "-find"):
	print("(detect -find flag, use original formula.)\n")
else:
	print("(**no optional flag detected, perform BMC with negated formula.\n")



# get all tid after all the quantifiers
tid_list = re.findall("[a-z, A-Z]\.", text)
#TODO: allow non-alphabetic path variable names
# tid_list = re.findall("[a-z, A-Z]+\.", text)


## make '!' as "~" for genqbf
text = text.replace("!","~")




## checking if all are valid symbol
# all_vars = re.findall("[\.a-z0-9\w-]+\[", text)
# print(all_vars)
# for var in all_vars:
# 	var = var.replace("[","")
# 	print(var)
# 	if(var not in valid_variables ):
# 		print("[ !!! HyperQube error: the variable: ", var, ", is not a valid variable. ]")
# 		print("All valid varialbes include state and defined vars are:")
# 		print(valid_variables)
# 		quit()


binary_ops = re.findall("\(.*?\)", text)
for bins in binary_ops:
	# print("??????????????????????????????????")
	# print(bins)
	# if it's not a numerical comparison
	if ("*" not in bins):
		# print(bins)
		all_vars = re.findall("[a-z0-9\w]+\[", bins)
		# print(all_vars)
		for var in all_vars:
			var = var.replace("[","")
			if(var in bitblasting_dict):
				print("[ !!! HyperQube error: the variable: ", var, ", is a numerical variable and thus, is not a valid variable for binary operation. ]")
				print("use syntax *<vid>[<tid>] <arith_comp> <vid>[<tid>]* for numerical comparisons.")
				quit()




# fornow, genQBF suppots two quantifiers, subsitute all [...] into _...
characters = ["_A", "_B"]
for i in range(len(tid_list)):
	tid = tid_list[i].replace(".","")
	text = re.sub("\["+tid+"\]", characters[i], text)





arith_ops = re.findall("\*.*?\*", text)
for op in arith_ops:
	blasted = ""
	# print(op)
	op = op.replace("*", "")
	if("!="in op):
		vars = op.split("!=")
	elif ("="in op):
		vars = op.split("=")
	else:
		print("[ !!! HyperQube error: arithmetic comparison is not correctly constructed. Use '=' or '!=' to connect two numerical variables. ]")
		quit()

	var_l = str(vars[0]).rsplit('_', 1)
	var_r = str(vars[1]).rsplit('_', 1)

	try:
		if (var_l[0].isdigit() and var_r[0].isdigit()):
			print("[ !!! HyperQube error: arithmetic operation is not correctly constructed. ]")
			quit()

		elif (var_l[0].isdigit()):
			blasted = binary_assign(var_r, int(var_r[0]))

		elif (var_r[0].isdigit()):
			blasted = binary_assign(var_l, int(var_r[0]))

		else:
			num_bits_left = bitblasting_dict[var_l[0].replace(" ","")]
			num_bits_right = bitblasting_dict[var_r[0].replace(" ","")]
			if(num_bits_left != num_bits_right):
				print("[ !!! HyperQube error: arithmetic conparison requires two variables with same number of bits in binary representation. ]")
				quit()
			else:
				# print("blasting")
				if("!="in op):
					# print("~"+binary_eq(var_l, var_r))
					blasted = "~"+binary_eq(var_l, var_r)
				else:
					# print(binary_eq(var_l, var_r))
					blasted = binary_eq(var_l, var_r)

		text = text.replace(op, blasted)
	except KeyError:
		print("[ !!! HyperQube error: arithmetic comparison is not correctly constructed. Boolean variables are not allowed in the comparison. ] ")
		quit()



# read quantifier selection, store in QS.hq
QUANTIFIER_1 = text.split(" ", 0)
Quants=""
if (re.findall("exists.*?exists.*?", text)):
	Quants="QS=EE"
	if(To_Negate_formula):
		Quants="QS=AA"
elif (re.findall("forall.*?forall.*?", text)):
	Quants="QS=AA"
	if(To_Negate_formula):
		Quants="QS=EE"
elif (re.findall("exists.*?forall.*?", text)):
	Quants="QS=EA"
	if(To_Negate_formula):
		Quants="QS=AE"
elif (re.findall("forall.*?exists.*?", text)):
	Quants="QS=AE"
	if(To_Negate_formula):
		Quants="QS=EA"
else:
	print("[ error: invald format of quantifiers and path variables. ]")
	print("correct format {exists, forall} {var1_name}. {exists, forall} {var2_name}. ")

QS = open(QS_file_name, "w")
QS.write(Quants)
QS.close()
# clea up quantifiers
text = text.replace("forall","")
text = text.replace("exists","")


text = re.sub("[a-z, A-Z]\.", "", text)
# clean up arith operations
text = text.replace("*","")
text = remove_dot(text)
text = text.replace(" ","")


if(To_Negate_formula):
	text= "~("+ text + ")"

### finally
# print("formula translated into Boolean representation: \n" + Quants + ", " + text)

def gen_P():
	##  write to R_bool file
	P_bool = open(translated_formula_file_name, "w")
	P_bool.write(text)
	P_bool.close()

gen_P()
# print("[ success! input formula translated into Boolean Expressions: test_P.bool ]")
print("[ success! input formula translated into Boolean Expressions.]")
