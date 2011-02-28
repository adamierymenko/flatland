#!/usr/bin/ruby

$x = 0.0
$y = 0.0

def num1
	return Math::cos($x += (Math::PI / 1000.0))
end

def num2
	return rand
end

10000.times {
	puts "#{num1} #{num2} #{num1} #{num2} #{num1} #{num2} #{num1} #{num2} #{num1} #{num2} #{num1} #{num2} #{num1} #{num2} #{num1} #{num2}"
}
