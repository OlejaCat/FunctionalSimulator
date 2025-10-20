require_relative "../src/dsl/assembler"

puts "Test of DSL"

prog = init_assembler do
  label :loop
  add :r4, :r1, :r2
  add :r1, :r2, 0
  add :r2, :r4, 0
  add :r3, :r3, :r5
  bne(:r3, 0, :loop)
  
end

puts prog.to_hex
write_binary("examples/fib.bin", prog)
