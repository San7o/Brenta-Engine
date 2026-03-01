-- Circle around the X axis

print("Hello, lua!")

time = 0

function update(delta)
   time = time + delta

   local x, y, z = get_position()
   set_position(x, math.cos(time), math.sin(time))
end
