import sys
fl="/home/dominykas/c++/FSim/Assets/Models/Levels/level.cfg"
file=open(fl,"w")
file.write("Lighting:\n")
for i in bpy.data.objects["Lighting"].children:
	file.write(i.name+":"+str(i.location.x)+","+str(i.location.z)+","+str(i.location.y)+","+str(i.data.color[0])+","+str(i.data.color[1])+","+str(i.data.color[2])+",\n")
file.write("Structures:\n")
for i in bpy.data.objects["Structures"].children:
	file.write(i.name+":"+str(i.location.x)+","+str(i.location.z)+","+str(i.location.y)+","+str(i.rotation_quaternion.w)+","+str(i.rotation_quaternion.x)+","+str(i.rotation_quaternion.y)+","+str(i.rotation_quaternion.z)+",\n")
file.close()
