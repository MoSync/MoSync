puts "Hello Ruby!"

class Person
  attr_accessor :name, :age
  def initialize(name, age)
    @name, @age = name, age
  end
  def to_s
    "my name is #{name} and I'm #{age} years old"
  end
end


def fib(n)
  if n < 2
    1
  else
   fib(n-1) + fib(n-2)
  end
end

(0..10).each { |n| puts fib(n) }

p = Person.new("olle", "42")

puts p

