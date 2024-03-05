MyCharacterScript = Character:new()

function MyCharacterScript:Start()
	MyCharacterScript:BindInputKey("MoveForward", KeyPressType["REPEAT"], Keys["KEY_W"])
	MyCharacterScript:BindInputKey("MoveBackward", KeyPressType["REPEAT"], Keys["KEY_S"])
	MyCharacterScript:BindInputKey("MoveLeft", KeyPressType["REPEAT"], Keys["KEY_A"])
	MyCharacterScript:BindInputKey("MoveRight", KeyPressType["REPEAT"], Keys["KEY_D"])
	MyCharacterScript:BindInputKey("Jump", KeyPressType["PRESS"], Keys["KEY_SPACE"])
end

function MyCharacterScript:Update()
	--MyCharacterScript:TestPrint()
end	