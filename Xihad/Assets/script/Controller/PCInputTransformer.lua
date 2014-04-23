local PCInputTransformer = {}
PCInputTransformer.__index = PCInputTransformer

function PCInputTransformer.new()
	local obj = setmetatable({}, PCInputTransformer)
	--init object field
	
	return obj
end

function PCInputTransformer:onMouseEvent( e )
	if e.type == 'rUplift' then
		
	elseif e.type == 'lClicked' then
		
	end
	
	return 0
end

return PCInputTransformer
