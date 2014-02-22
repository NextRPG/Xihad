local t1 = coroutine.create(function ()
	local t2 = coroutine.create(function (ret)
		print("enter t2")
		print(coroutine.resume(ret))
		print("return to t2")
	end)
	
	print(coroutine.resume(t2, coroutine.running()))
	print("return to t1")
	print(coroutine.resume(t2))
	print("end of t1")
end)

coroutine.resume(t1)