
fin = open('onlyAccelPoints.txt', 'r')

fout = open('accelVelocityPosition.txt', 'w')

PositionX = 0
VelocityX = 0
AccelX = 0

pastAccel = 0
pastTime = 0
currentTime = 0
lastTime = 0

upper = 5000
offset = 10

index = 0

gravity = 9.8
	
for line in fin:

	#extracting time and accel from text file
	currentTime, currentAccel = line.split(',')
	currentTime = float(currentTime.strip())
	currentAccel = float(currentAccel.strip())*gravity	#convert acc from g's to m/s^2

	#only Keeping first 4 digits of accel.
	currentAccel = (float(int(currentAccel*10)))/10
	

	if index%2 == 0:
		pastAccel = currentAccel
		#print "CABO --> for loop :: index skip"

	else:
		currentAccel = (currentAccel+pastAccel)/2
		#print "CABO --> odd loop :: calculating accel"

		#calculating dt, converting from microseconds to seconds
		delta_t = (currentTime - lastTime)/1000000

		#integrating acceleration to get velocity, using trapezoids
		delta_acc = (currentAccel + AccelX)/2
		delta_vel = delta_t*delta_acc
		newVelocity = VelocityX+delta_vel
		

		#integrating velocity to get position
		delta_vel = (newVelocity + VelocityX)/2
		delta_pos = delta_t*delta_vel
		newPosition = PositionX+delta_pos

		#bleh data

		if index <10:
			print "Current Time (microS): ", currentTime
			print "Current Time (s):  ", currentTime/1000000
			print "Index              ", index
			print "Acceleration:      ", currentAccel 
			print "Velocity:          ", newVelocity
			print "Position:          ", newPosition
			print "-----------------------------------"

		if index == upper:
			print "*********************************************************"

		if index >upper and index < upper+offset:
			print "Current Time (microS): ", currentTime
			print "Current Time (s):  ", currentTime/1000000
			print "Index              ", index
			print "Acceleration:      ", currentAccel 
			print "Velocity:          ", newVelocity
			print "Position:          ", newPosition
			print "-----------------------------------"

		#updating stuff for next cycle
		lastTime = currentTime
		AccelX = currentAccel
		VelocityX = newVelocity
		PositionX = newPosition

	index = index+1


print index
fin.close()
fout.close()