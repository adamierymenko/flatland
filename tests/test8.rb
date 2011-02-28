#!/usr/bin/ruby

n = -(Math::PI * 2.0)
while (n <= (Math::PI * 2.0))
	puts "#{n} #{Math::sin(n)} #{n}"
	n += 0.01
end
