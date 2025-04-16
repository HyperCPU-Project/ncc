#include <backend/qproc/qproc_backend.hpp>

QprocBackend::QprocBackend() {
	types["void"] = TypeMetadata("void", 0);
	types["int"] = TypeMetadata("int", 4);
	reg_index = 0;
}

std::string QprocBackend::emit_function_prologue() {
	return "push bp\nmov bp sp";
}

std::string QprocBackend::emit_function_epilogue() {
	return "mov bp sp\npop bp\npop ip";
}

std::string QprocBackend::emit_mov_const(int32_t constant) {
	std::string code = fmt::format("movi r{} {}\n", reg_index, constant);
	Value value(fmt::format("r{}", reg_index));
	values.push(value);
	previous_reg_indexes.push(reg_index);
	reg_index++;
	if (reg_index > 12) {
		reg_index = 0;
	}
	return code;
}

std::string QprocBackend::emit_add(std::string ROUT, std::string RLHS, std::string RRHS) {
	// return fmt::format("movi {} {}\nadd {} {}\n", ROUT, RLHS, ROUT, RRHS);
	// return fmt::format("add {} {}\n", RLHS, RRHS);
	if (ROUT != RLHS) {
		return fmt::format("add {} {}\nmov {} {}\n", RLHS, RRHS, ROUT, RLHS);
	}
	return fmt::format("add {} {}\n", RLHS, RRHS);
}

std::string QprocBackend::emit_mul(std::string ROUT, std::string RLHS, std::string RRHS) {
	// return fmt::format("movi {} {}\nmul {} {}\n", ROUT, RLHS, ROUT, RRHS);
	if (ROUT != RLHS) {
		return fmt::format("mul {} {}\nmov {} {}\n", RLHS, RRHS, ROUT, RLHS);
	}
	return fmt::format("mul {} {}\n", RLHS, RRHS);
}

std::string QprocBackend::emit_entrypoint() {
	return R"(b ncc_start_main
ncc_start_main:
call main
qdb
hlt

)";
}
