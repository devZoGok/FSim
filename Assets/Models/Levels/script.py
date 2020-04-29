import sys
fl="/home/dominykas/c++/FSim/Assets/Models/Levels/level.cfg"
file=open(fl,"w")
file.write("Lighting:\n")
for i in bpy.data.objects["Lighting"].children:
	file.write(i.name+","+str(i.location.x)+","+str(i.location.z)+","+str(i.location.y)+","+str(i.data.color[0])+","+str(i.data.color[1])+","+str(i.data.color[2])+",\n")
file.write("Structures:\n")
for i in bpy.data.objects["ChineseStructures"].children:
	file.write(i.name+",0,"+str(i.location.x)+","+str(i.location.z)+","+str(-i.location.y)+","+str(i.rotation_quaternion.w)+","+str(i.rotation_quaternion.x)+","+str(i.rotation_quaternion.y)+","+str(i.rotation_quaternion.z)+",\n")
for i in bpy.data.objects["JapaneseStructures"].children:
	file.write(i.name+",1,"+str(i.location.x)+","+str(i.location.z)+","+str(-i.location.y)+","+str(i.rotation_quaternion.w)+","+str(i.rotation_quaternion.x)+","+str(i.rotation_quaternion.y)+","+str(i.rotation_quaternion.z)+",\n")
for i in bpy.data.objects["KoreanStructures"].children:
	file.write(i.name+",2,"+str(i.location.x)+","+str(i.location.z)+","+str(-i.location.y)+","+str(i.rotation_quaternion.w)+","+str(i.rotation_quaternion.x)+","+str(i.rotation_quaternion.y)+","+str(i.rotation_quaternion.z)+",\n")
spawn=bpy.data.objects["Spawn"]
file.write("Objectives:\nSpawn:\n-1,-1,-1,"+str(spawn.location.x)+","+str(spawn.location.z)+","+str(-spawn.location.y)+","+str(spawn.rotation_quaternion.w)+","+str(spawn.rotation_quaternion.x)+","+str(spawn.rotation_quaternion.y)+","+str(spawn.rotation_quaternion.z)+",")
file.close()
