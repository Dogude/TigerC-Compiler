---@diagnostic disable: undefined-global

--[[
"cmd": "windres resources.rc -o res.o && tcc src/*.c res.o main.c -o output.exe -luser32 && output.exe "
"cmd" : "nasm -f win64 test.asm -o test.o && gcc test.o -mwindows -nostdlib -s -o test.exe -Wl,-e,main -L\"C:\\Users\\karad\\OneDrive\\Belgeler\\mingw64\\lib\" -lmsvcrt && test.exe" ,
--]]
local source = process_source('code.c')

Token = {
		
		location = 0,
		precedence = 4,
		
		tokens = getTokens(),
		count = tokenCount() - 1,
		size = tokenSize()

}

-- Storing define'd names
local define_look_up = {}

-- Storing all types of names except defines
local names = {}

-- process expressions / Generate Machine Code
local execution_stack = {}           

-- x86 machine op_codes / this will go linker
local code = {}

for i = 0 , Token.count do -- process bytecode
	local op_code = fetch(Token.tokens,(i*Token.size + Token.precedence),'int')
	if op_code == 199 then
		local location = fetch(Token.tokens,(i*Token.size + Token.location),'int')
		local sub = source:sub(location + #'define ')		
		local target = sub:match("(%w+)")
		local target2 = sub:match("%s+(%w+)")
		define_look_up[target] = target2
		

	elseif op_code == 198 then
		local location = fetch(Token.tokens,(i*Token.size + Token.location),'int')
		local sub = source:sub(location + #'ifdef ')		
		local target = sub:match("(%w+)")
		if not define_look_up[target] then 
			while i < Token.count do
				local op_code = fetch(Token.tokens,(i*Token.size + Token.precedence),'int')
				if op_code == 196 or op_code == 195 then
					 break 
				else
					i = i + 1		
				end
			end
		end

	elseif op_code == 197 then
		local location = fetch(Token.tokens,(i*Token.size + Token.location),'int')
		local sub = source:sub(location + #'ifndef ')		
		local target = sub:match("(%w+)")
		if define_look_up[target] then 
			while i < Token.count do
				local op_code = fetch(Token.tokens,(i*Token.size + Token.precedence),'int')
				if op_code == 196 or op_code == 195 then
					 break 
				else
					i = i + 1		
				end
			end
		end

	elseif op_code == 195 then
		
		
	elseif op_code == 198 then
		
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	
	elseif op_code == 198 then
	

	end


end	



release(source)

