extends KinematicBody


# Declare member variables here. Examples:
onready var camera_rig = $camera_rig


# Called when the node enters the scene tree for the first time.
func _ready():
	camera_rig.set_as_toplevel(true)


# Called every frame. 'delta' is the elapsed time since the previous frame.

func _physics_process(delta):
	var quad_pos = global_transform.origin
	camera_rig.global_transform.origin = quad_pos


