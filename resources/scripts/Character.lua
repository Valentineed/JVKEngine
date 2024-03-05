local class = require "middleclass"

Character = class("Character")

function Character:initialize()
	print("Character Created")
end

function Character:speak()
	print("Character speak")
end

function Character:TestPrint()
	print("Print from Lua")	
end

function Character:TestPrint2() -- Bind
	TestPrint2() -- Bind
end

function Character:BindInputKey(action, keyPressType, keyCode) -- Bind
	print("Calling BindInputKey from Lua sucessfull", action, keyPressType , keyCode)
	BindInputKey(action, keyPressType, keyCode) -- Bind
end

return Character
