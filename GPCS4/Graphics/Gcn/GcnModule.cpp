#include "GcnModule.h"
#include "GcnAnalysis.h"
#include "GcnCompiler.h"
#include "GcnDecoder.h"


using namespace sce::vlt;

namespace sce::gcn
{
	GcnModule::GcnModule(
		GcnProgramType              type,
		const std::vector<uint8_t>& code) :
		m_programInfo(type),
		m_header(code),
		m_code(code)
	{
	}

	GcnModule::~GcnModule()
	{
	}

	Rc<VltShader> GcnModule::compile() const
	{
		const uint32_t* start = reinterpret_cast<const uint32_t*>(m_code.data());
		const uint32_t* end   = reinterpret_cast<const uint32_t*>(m_code.data() + m_code.size());
		GcnCodeSlice    codeSlice(start, end);

		GcnAnalysisInfo analysisInfo;

		GcnAnalyzer analyzer(
			m_programInfo, analysisInfo);

		this->runAnalyzer(analyzer, codeSlice);

		GcnCompiler compiler(
			m_header.key().name(), 
			m_programInfo,
			m_header,
			analysisInfo);

		this->runCompiler(compiler, codeSlice);
	}

	void GcnModule::runAnalyzer(
		GcnAnalyzer& analyzer,
		GcnCodeSlice slice) const
	{
		GcnDecodeContext decoder;

		while (!slice.atEnd())
		{
			decoder.decodeInstruction(slice);

			analyzer.processInstruction(
				decoder.getInstruction());
		}
	}

	void GcnModule::runCompiler(
		GcnCompiler& compiler,
		GcnCodeSlice slice) const
	{
		GcnDecodeContext decoder;

		while (!slice.atEnd())
		{
			decoder.decodeInstruction(slice);

			compiler.processInstruction(
				decoder.getInstruction());
		}
	}

	void GcnModule::applyFetchShader(
		const std::vector<uint8_t>& code)
	{
		GcnFetchShader fsShader(code);
		m_vsInputSemanticTable = fsShader.getVertexInputSemanticTable();
	}

}  // namespace sce::gcn