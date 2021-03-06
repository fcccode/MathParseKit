/*!
 * \file
 * \author Carlo Bernaschina (www.bernaschina.com)
 * \copyright Copyright 2013 Carlo Bernaschina. All rights reserved.
 * \license This project is released under the GNU Lesser General Public License.
 */

#include "MFAdd.h"
#include "MFConst.h"
#include <sstream>

using namespace mpk;

MFAdd::MFAdd(MFunction *lhs,MFunction *rhs){
	if (lhs) m_lhs=lhs->Clone();
	else m_lhs=NULL;
	if (rhs) m_rhs=rhs->Clone();
	else m_rhs=NULL;
	m_type=MF_ADD;
}

MFunction* MFAdd::Clone() const{
	return new MFAdd(m_lhs,m_rhs);
}

bool MFAdd::IsOk() const{
	if (!m_lhs) return false;
	if (!m_rhs) return false;
	if (!m_lhs->IsOk()) return false;
	if (!m_rhs->IsOk()) return false;
	return true;
}

bool MFAdd::IsConstant(MVariablesList* variables) const{
	if (m_lhs)
		if(!m_lhs->IsConstant(variables)) return false;
	if (m_rhs)
		if(!m_rhs->IsConstant(variables)) return false;
	return true;
}

MFunction* MFAdd::Solve(MVariablesList* variables) const{
	if (!m_lhs || !m_rhs) return new MFConst(0.0);
	MFunction *lhs=m_lhs->Solve(variables);
	MFunction *rhs=m_rhs->Solve(variables);
	if (lhs->GetType()==MF_CONST && rhs->GetType()==MF_CONST){
		double value=((MFConst*)lhs)->GetValue()+((MFConst*)rhs)->GetValue();
		lhs->Release();
		rhs->Release();
		return new MFConst(value);
	}
	MFAdd *ret=new MFAdd();
	ret->SetLhs(lhs);
	ret->SetRhs(rhs);
	return ret;
}

MFunction* MFAdd::Derivate(MVariablesList *variables) const{
	if (!m_lhs || !m_rhs) return NULL;
	if (m_lhs->IsConstant(variables)){
		if (m_rhs->IsConstant(variables)){
			return new MFConst(0.0);
		}else{
			return m_rhs->Derivate(variables);
		}
	}else{
		if (m_rhs->IsConstant(variables)){
			return m_lhs->Derivate(variables);
		}else{
			MFunction *lhs=m_lhs->Derivate(variables);
			if (!lhs) return NULL;
			MFunction *rhs=m_rhs->Derivate(variables);
			if (!rhs) return NULL;
			MFAdd *ret=new MFAdd();
			ret->SetLhs(lhs);
			ret->SetRhs(rhs);
			return ret;
		}
	}
}

MVariablesList* MFAdd::GetVariablesList(MVariablesList *list) const{
	if (!m_lhs || !m_rhs) return list;
	list = m_lhs->GetVariablesList(list);
	return m_rhs->GetVariablesList(list);
}

MSistem* MFAdd::GetDomain(MSistem *update) const{
	if (!m_lhs || !m_rhs) return update;
	update = m_lhs->GetDomain(update);
	return m_rhs->GetDomain(update);
}

void MFAdd::SetLhs(MFunction *lhs){
	if (m_lhs) m_lhs->Release();
	m_lhs=lhs;
}

void MFAdd::SetRhs(MFunction *rhs){
	if (m_rhs) m_rhs->Release();
	m_rhs=rhs;
}

std::wstring MFAdd::ToString() const {
	std::wostringstream stream;
	stream << m_lhs->ToString();
	stream << L"+";
	stream << m_rhs->ToString();
	return stream.str();
}

void MFAdd::Release(){
	if (m_lhs) m_lhs->Release();
	if (m_rhs) m_rhs->Release();
	delete this;
}