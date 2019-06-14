#include "condition.h"

namespace interpreter {

	// Смысл команд аналогичен ASM'скому и читается так:
	// значение1 COP значение2
	// COP:
	//	e  - equal			( == )
	//	ne - not equal		( != )
	//	b  - below			( <  )
	//	be - below or equal	( <= )
	//	l  - less			( <  ) для чисел со знаком
	//	le - less or equal	( <= ) для чисел со знаком

	//------------------------------------------------------------------------------
	// ~E
	template <typename T>
	inline void cmpeT(CVMState& vmState, const T value1, const T value2)
	{
		vmState.m_vmProcessor.bct = (value1 == value2);
	}

	//------------------------------------------------------------------------------
	// ~NE
	template <typename T>
	inline void cmpneT(CVMState& vmState, const T value1, const T value2)
	{
		vmState.m_vmProcessor.bct = (value1 != value2);
	}

	//------------------------------------------------------------------------------
	// ~B или ~L
	template <typename T>
	inline void cmpblT(CVMState& vmState, const T value1, const T value2)
	{
		vmState.m_vmProcessor.bct = (value1 < value2);
	}

	//------------------------------------------------------------------------------
	// ~BE или ~LE
	template <typename T>
	inline void cmpbelT(CVMState& vmState, const T value1, const T value2)
	{
		vmState.m_vmProcessor.bct = (value1 <= value2);
	}

	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CMPE
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	void cmpe8mm	(CVMState& vmState)
	{
		const byte value1 = *vmState.Read_8m();
		const byte value2 = *vmState.Read_8m();

		cmpeT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpe8mv	(CVMState& vmState)
	{
		const byte value1 = *vmState.Read_8m();
		const byte value2 = vmState.Read_8v();

		cmpeT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpe8vm	(CVMState& vmState)
	{
		const byte value1 = vmState.Read_8v();
		const byte value2 = *vmState.Read_8m();

		cmpeT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpe16mm	(CVMState& vmState)
	{
		const word value1 = *vmState.Read_16m();
		const word value2 = *vmState.Read_16m();

		cmpeT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpe16mv	(CVMState& vmState)
	{
		const word value1 = *vmState.Read_16m();
		const word value2 = vmState.Read_16v();

		cmpeT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpe16vm	(CVMState& vmState)
	{
		const word value1 = vmState.Read_16v();
		const word value2 = *vmState.Read_16m();

		cmpeT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpe32mm	(CVMState& vmState)
	{
		const dword value1 = *vmState.Read_32m();
		const dword value2 = *vmState.Read_32m();

		cmpeT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpe32mv	(CVMState& vmState)
	{
		const dword value1 = *vmState.Read_32m();
		const dword value2 = vmState.Read_32v();

		cmpeT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpe32vm	(CVMState& vmState)
	{
		const dword value1 = vmState.Read_32v();
		const dword value2 = *vmState.Read_32m();

		cmpeT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CMPNE
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	void cmpne8mm	(CVMState& vmState)
	{
		const byte value1 = *vmState.Read_8m();
		const byte value2 = *vmState.Read_8m();

		cmpneT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpne8mv	(CVMState& vmState)
	{
		const byte value1 = *vmState.Read_8m();
		const byte value2 = vmState.Read_8v();

		cmpneT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpne8vm	(CVMState& vmState)
	{
		const byte value1 = vmState.Read_8v();
		const byte value2 = *vmState.Read_8m();

		cmpneT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpne16mm	(CVMState& vmState)
	{
		const word value1 = *vmState.Read_16m();
		const word value2 = *vmState.Read_16m();

		cmpneT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpne16mv	(CVMState& vmState)
	{
		const word value1 = *vmState.Read_16m();
		const word value2 = vmState.Read_16v();

		cmpneT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpne16vm	(CVMState& vmState)
	{
		const word value1 = vmState.Read_16v();
		const word value2 = *vmState.Read_16m();

		cmpneT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpne32mm	(CVMState& vmState)
	{
		const dword value1 = *vmState.Read_32m();
		const dword value2 = *vmState.Read_32m();

		cmpneT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpne32mv	(CVMState& vmState)
	{
		const dword value1 = *vmState.Read_32m();
		const dword value2 = vmState.Read_32v();

		cmpneT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------
	void cmpne32vm	(CVMState& vmState)
	{
		const dword value1 = vmState.Read_32v();
		const dword value2 = *vmState.Read_32m();

		cmpneT( vmState, value1, value2 );
	}

	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CMPB
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	void cmpb8mm (CVMState& vmState)
	{
		const byte value1 = *vmState.Read_8m();
		const byte value2 = *vmState.Read_8m();

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpb8mv (CVMState& vmState)
	{
		const byte value1 = *vmState.Read_8m();
		const byte value2 = vmState.Read_8v();

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpb8vm (CVMState& vmState)
	{
		const byte value1 = vmState.Read_8v();
		const byte value2 = *vmState.Read_8m();

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpb16mm (CVMState& vmState)
	{
		const word value1 = *vmState.Read_16m();
		const word value2 = *vmState.Read_16m();

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpb16mv (CVMState& vmState)
	{
		const word value1 = *vmState.Read_16m();
		const word value2 = vmState.Read_16v();

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpb16vm (CVMState& vmState)
	{
		const word value1 = vmState.Read_16v();
		const word value2 = *vmState.Read_16m();

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpb32mm (CVMState& vmState)
	{
		const dword value1 = *vmState.Read_32m();
		const dword value2 = *vmState.Read_32m();

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpb32mv (CVMState& vmState)
	{
		const dword value1 = *vmState.Read_32m();
		const dword value2 = vmState.Read_32v();

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpb32vm (CVMState& vmState)
	{
		const dword value1 = vmState.Read_32v();
		const dword value2 = *vmState.Read_32m();

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CMPBE
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	void cmpbe8mm (CVMState& vmState)
	{
		const byte value1 = *vmState.Read_8m();
		const byte value2 = *vmState.Read_8m();

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpbe8mv (CVMState& vmState)
	{
		const byte value1 = *vmState.Read_8m();
		const byte value2 = vmState.Read_8v();

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpbe8vm (CVMState& vmState)
	{
		const byte value1 = vmState.Read_8v();
		const byte value2 = *vmState.Read_8m();

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpbe16mm (CVMState& vmState)
	{
		const word value1 = *vmState.Read_16m();
		const word value2 = *vmState.Read_16m();

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpbe16mv (CVMState& vmState)
	{
		const word value1 = *vmState.Read_16m();
		const word value2 = vmState.Read_16v();

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpbe16vm (CVMState& vmState)
	{
		const word value1 = vmState.Read_16v();
		const word value2 = *vmState.Read_16m();

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpbe32mm (CVMState& vmState)
	{
		const dword value1 = *vmState.Read_32m();
		const dword value2 = *vmState.Read_32m();

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpbe32mv (CVMState& vmState)
	{
		const dword value1 = *vmState.Read_32m();
		const dword value2 = vmState.Read_32v();

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpbe32vm (CVMState& vmState)
	{
		const dword value1 = vmState.Read_32v();
		const dword value2 = *vmState.Read_32m();

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CMPL
	//------------------------------------------------------------------------------

	inline sbyte ConvertToSigned( const byte value )
	{
		return *((sbyte*)&value);
	}

	inline sword ConvertToSigned( const word value )
	{
		return *((sword*)&value);
	}

	inline sdword ConvertToSigned( const dword value )
	{
		return *((sdword*)&value);
	}

	//------------------------------------------------------------------------------
	void cmpl8mm (CVMState& vmState)
	{
		const sbyte value1 = *reinterpret_cast< sbyte* >( vmState.Read_8m() );
		const sbyte value2 = *reinterpret_cast< sbyte* >( vmState.Read_8m() );

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpl8mv (CVMState& vmState)
	{
		const sbyte value1 = *reinterpret_cast< sbyte* >( vmState.Read_8m() );
		const sbyte value2 = ConvertToSigned( vmState.Read_8v() );

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpl8vm (CVMState& vmState)
	{
		const sbyte value1 = ConvertToSigned( vmState.Read_8v() );
		const sbyte value2 = *reinterpret_cast< sbyte* >( vmState.Read_8m() );

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpl16mm (CVMState& vmState)
	{
		const sword value1 = *reinterpret_cast< sword* >( vmState.Read_16m() );
		const sword value2 = *reinterpret_cast< sword* >( vmState.Read_16m() );

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpl16mv (CVMState& vmState)
	{
		const sword value1 = *reinterpret_cast< sword* >( vmState.Read_16m() );
		const sword value2 = ConvertToSigned( vmState.Read_16v() );

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpl16vm (CVMState& vmState)
	{
		const sword value1 = ConvertToSigned( vmState.Read_16v() );
		const sword value2 = *reinterpret_cast< sword* >( vmState.Read_16m() );

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpl32mm (CVMState& vmState)
	{
		const sdword value1 = *reinterpret_cast< sdword* >( vmState.Read_32m() );
		const sdword value2 = *reinterpret_cast< sdword* >( vmState.Read_32m() );

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpl32mv (CVMState& vmState)
	{
		const sdword value1 = *reinterpret_cast< sdword* >( vmState.Read_32m() );
		const sdword value2 = ConvertToSigned( vmState.Read_32v() );

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmpl32vm (CVMState& vmState)
	{
		const sdword value1 = ConvertToSigned( vmState.Read_32v() );
		const sdword value2 = *reinterpret_cast< sdword* >( vmState.Read_32m() );

		cmpblT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CMPLE
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	void cmple8mm (CVMState& vmState)
	{
		const sbyte value1 = *reinterpret_cast< sbyte* >( vmState.Read_8m() );
		const sbyte value2 = *reinterpret_cast< sbyte* >( vmState.Read_8m() );

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmple8mv (CVMState& vmState)
	{
		const sbyte value1 = *reinterpret_cast< sbyte* >( vmState.Read_8m() );
		const sbyte value2 = ConvertToSigned( vmState.Read_8v() );

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmple8vm (CVMState& vmState)
	{
		const sbyte value1 = ConvertToSigned( vmState.Read_8v() );
		const sbyte value2 = *reinterpret_cast< sbyte* >( vmState.Read_8m() );

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmple16mm (CVMState& vmState)
	{
		const sword value1 = *reinterpret_cast< sword* >( vmState.Read_16m() );
		const sword value2 = *reinterpret_cast< sword* >( vmState.Read_16m() );

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmple16mv (CVMState& vmState)
	{
		const sword value1 = *reinterpret_cast< sword* >( vmState.Read_16m() );
		const sword value2 = ConvertToSigned( vmState.Read_16v() );

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmple16vm (CVMState& vmState)
	{
		const sword value1 = ConvertToSigned( vmState.Read_16v() );
		const sword value2 = *reinterpret_cast< sword* >( vmState.Read_16m() );

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmple32mm (CVMState& vmState)
	{
		const sdword value1 = *reinterpret_cast< sdword* >( vmState.Read_32m() );
		const sdword value2 = *reinterpret_cast< sdword* >( vmState.Read_32m() );

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmple32mv (CVMState& vmState)
	{
		const sdword value1 = *reinterpret_cast< sdword* >( vmState.Read_32m() );
		const sdword value2 = ConvertToSigned( vmState.Read_32v() );

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------
	void cmple32vm (CVMState& vmState)
	{
		const sdword value1 = ConvertToSigned( vmState.Read_32v() );
		const sdword value2 = *reinterpret_cast< sdword* >( vmState.Read_32m() );

		cmpbelT(vmState, value1, value2);
	}

	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// JCx
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	void jct32v (CVMState& vmState)
	{
		const dword addr = vmState.Read_32v();

		if (vmState.m_vmProcessor.bct)
		{
			vmState.JumpTo( addr );
		}
	}

	//------------------------------------------------------------------------------
	void jct32m (CVMState& vmState)
	{
		const dword addr = *vmState.Read_32m();

		if (vmState.m_vmProcessor.bct)
		{
			vmState.JumpTo( addr );
		}
	}

	//------------------------------------------------------------------------------
	void jcf32v (CVMState& vmState)
	{
		const dword addr = vmState.Read_32v();

		if (!vmState.m_vmProcessor.bct)
		{
			vmState.JumpTo( addr );
		}
	}

	//------------------------------------------------------------------------------
	void jcf32m (CVMState& vmState)
	{
		const dword addr = *vmState.Read_32m();

		if (!vmState.m_vmProcessor.bct)
		{
			vmState.JumpTo( addr );
		}
	}

	//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

}