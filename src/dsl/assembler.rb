class Assembler
  Registers = (0..31).map { |i| ["r#{i}".to_sym, i]}.to_h

  Opcodes = { 
    :NOR     => 0b001101,
    :LDP     => 0b111100,
    :CBIT    => 0b111001,
    :BDEP    => 0b110011,
    :ADD     => 0b011010,
    :SSAT    => 0b001100,
    :ST      => 0b110010,
    :CLZ     => 0b110101,
    :BNE     => 0b000110,
    :LD      => 0b001010,
    :XOR     => 0b101001,
    :SYSCALL => 0b111000
  }

  def initialize
    @instructions = []
    @labels = {}
    @address = 0
  end

  def get_register(register)
    case register
    when Integer
      register
    when String, Symbol
      reg_sym = register.is_a?(String) ? register.downcase.to_sym : register
      register_number = Registers[reg_sym]
      register_number
    else
      puts "Invalid register type"
    end
  end

  def r_format(opcode, rd, rs, rt)
    rd_index = get_register(rd)
    rs_index = get_register(rs)
    rt_index = get_register(rt)
    instruction = (rs_index << 21) | (rt_index << 16) | (rd_index << 11) | opcode
    @instructions << instruction
    @address += 4
  end

  def mem_format(opcode, rt, base, offset)
    rt_index = get_register(rt)
    base_index = get_register(base)
    instruction = (opcode << 26) | (base_index << 21) | (rt_index << 16) | (offset & 0xFFFF)
    @instructions << instruction
    @address += 4
  end

  def branch_format(opcode, rs, rt, label)
    rs_index = get_register(rs)
    rt_index = get_register(rt)
    @instructions << { type: :branch, opcode: opcode, rs: rs_index, rt: rt_index, label: label }
    @address += 4
  end

  def imm_format(opcode, rd, rs, immediate)
    rd_index = get_register(rd)
    rs_index = get_register(rs)
    instruction = (opcode << 26) | (rd_index << 21) | (rs_index << 16) | ((immediate & 0x1F) << 11)
    @instructions << instruction
    @address += 4
  end

  def ldp_format(opcode, rt1, rt2, base, offset)
    rt1_index = get_register(rt1)
    rt2_index = get_register(rt2)
    base_index = get_register(base)
    instruction = (opcode << 26) | (base_index << 21) | (rt1_index << 16) | (rt2_index << 11) | (offset & 0x7FF)
    @instructions << instruction
    @address += 4
  end

  def jump_format(opcode, label)
    @instructions << { type: :jump, opcode: opcode, label: label }
    @address += 4
  end

  def syscall_format(code = 0)
    instruction = (0x00 << 26) | ((code & 0xFFFFF) << 6) | Opcodes[:SYSCALL]
    @instructions << instruction
    @address += 4
  end

  def label(name)
    @labels[name] = @address
  end

  def nor(rd, rs, rt)
    r_format(Opcodes[:NOR], rd, rs, rt)
  end

  def add(rd, rs, rt)
    r_format(Opcodes[:ADD], rd, rs, rt)
  end

  def xor(rd, rs, rt) 
    r_format(Opcodes[:XOR], rd, rs, rt)
  end

  def clz(rd, rs)
    rd_index = get_register(rd)
    rs_index = get_register(rs)
    instruction = (rd_index << 21) | (rs_index << 16) | Opcodes[:CLZ]
    @instructions << instruction
    @address += 4
  end

  def bdep(rd, rs1, rs2)
    rd_index = get_register(rd)
    rs1_index = get_register(rs1)
    rs2_index = get_register(rs2)
    instruction = (rd_index << 21) | (rs1_index << 16) | (rs2_index << 11) | Opcodes[:BDEP]
    @instructions << instruction
    @address += 4
  end

  def ld(rt, offset, base)
    mem_format(Opcodes[:LD], rt, base, offset)
  end

  def st(rt, offset, base)
    mem_format(Opcodes[:ST], rt, base, offset)
  end

  def ldp(rt1, rt2, offset, base)
    ldp_format(Opcodes[:LDP], rt1, rt2, base, offset)
  end

  def bne(rs, rt, label)
    branch_format(Opcodes[:BNE], rs, rt, label)
  end

  def beq(rs, rt, label)
    branch_format(Opcodes[:BEQ], rs, rt, label)
  end

  def j(label)
    jump_format(Opcodes[:J], label)
  end

  def cbit(rd, rs, immediate)
    imm_format(Opcodes[:CBIT], rd, rs, immediate)
  end

  def ssat(rd, rs, immediate)
    imm_format(Opcodes[:SSAT], rd, rs, immediate)
  end

  def syscall(code = 0)
    syscall_format(code)
  end

  def to_binary
    collect_labels
    @instructions.pack('V*')
  end

  def to_hex
    collect_labels
    
    lookup = {
      [0x00, 0x0D] => "NOR", [0x00, 0x1A] => "ADD", [0x00, 0x29] => "XOR",
      [0x00, 0x35] => "CLZ", [0x00, 0x33] => "BDEP", [0x00, 0x38] => "SYSCALL",
      [0x0A, nil] => "LD", [0x32, nil] => "ST", [0x3C, nil] => "LDP",
      [0x06, nil] => "BNE", [0x1E, nil] => "BEQ", [0x36, nil] => "J",
      [0x39, nil] => "CBIT", [0x0C, nil] => "SSAT"
    }

    @instructions.map.with_index do |inst, i|
      opcode = (inst >> 26) & 0x3F
      funct = inst & 0x3F
      key = opcode == 0x00 ? [opcode, funct] : [opcode, nil]

      "#{'%3d' % i}: 0x#{'%08x' % inst}  //  #{lookup[key]}"
    end.join("\n")
  end

  private

  def collect_labels
    instructions_with_labels = []
    
    @instructions.each_with_index do |inst, index|
      if inst.is_a?(Hash)
        case inst[:type]
        when :branch
          target_address = @labels[inst[:label]]
          offset = (target_address - (index * 4)) >> 2
          current_inst = (inst[:opcode] << 26) | (inst[:rs] << 21) | (inst[:rt] << 16) | (offset & 0xFFFF)
          instructions_with_labels << current_inst
          
        when :jump
          target_address = @labels[inst[:label]]
          target_index = target_address >> 2
          current_inst = (inst[:opcode] << 26) | (target_index & 0x3FFFFFF)
          instructions_with_labels << current_inst
        end
      else
        instructions_with_labels << inst
      end
    end
    
    @instructions = instructions_with_labels
  end
end

def init_assembler(&block)
  dsl = Assembler.new
  dsl.instance_eval(&block)
  dsl
end

def write_binary(file, assembler)
  binary_data = assembler.to_binary
  File.binwrite(file, binary_data)
  puts "Binary file written"
end
