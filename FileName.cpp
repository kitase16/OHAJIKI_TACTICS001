#include <iostream>
#include <vector>
#include <cstdint>
#include <string>


struct BYTE;
struct OTINTEGER {
	std::vector<std::uint8_t> value;
};

struct BYTE {
	union {
		struct { std::uint8_t _; };
		union {
			std::int8_t Signed;
			std::uint8_t Unsigned;
			std::int8_t SArray[1];
			std::uint8_t UArray[1];
		};
	};

	const BYTE& operator=(const BYTE& other) {
		this->Unsigned = other.Unsigned;
		return *this;
	}	
};

struct Word {
	union {
		struct { std::uint16_t _; };
		struct { BYTE A, B; };
		union {
			std::int16_t Signed;
			std::uint16_t Unsigned;
			std::int16_t SArray[1];
			std::uint16_t UArray[1];
		};
		/** /const Word& operator=(const Word& other) {
			this->A.Unsigned = other.A.Unsigned;
			this->B.Unsigned = other.B.Unsigned;
			return (*this);
		}
		/**/
	};
};
OTINTEGER Add(const OTINTEGER& AAA, const OTINTEGER& BBB) {
	
	OTINTEGER A = AAA;
	OTINTEGER B = BBB;
	OTINTEGER C;

	if(A.value.size()==0) A.value.push_back(0);
	if(B.value.size()==0) B.value.push_back(0);

	std::size_t Lmin = ((A.value.size() <= B.value.size()) ? A.value.size() : B.value.size());
	std::size_t Lmax = ((A.value.size() > B.value.size()) ? A.value.size() : B.value.size());

	if(A.value.size()==1) A.value.push_back(0);
	if(B.value.size()==1) B.value.push_back(0);

	A.value.push_back(0);
	A.value.push_back(0);
	A.value.push_back(0);
	A.value.push_back(0);
	B.value.push_back(0);
	B.value.push_back(0);
	B.value.push_back(0);
	B.value.push_back(0);
	//C.value.push_back(0);
	//C.value.push_back(0);

	
	
	std::size_t i = 0;
	for (; i <= Lmin; i++){
		const Word* X = (Word*)(((BYTE*)((&A.value[i])+1))-1) ;
		const Word* Y = (Word*)(((BYTE*)((&B.value[i])+1))-1) ;

		Word Z;

		Z.Unsigned = X->Unsigned + Y->Unsigned;

		C.value.push_back(Z.A.Unsigned);
	}

	OTINTEGER* AA = ((A.value.size() >= B.value.size()) ? &A : &B);

	for (; i <= Lmax; i++) {
		Word* X = (Word*)(((BYTE*)((&(AA->value[i])+1)))-1) ;
		//const Word* Y = (const Word*)((&(B[0]._)) + 1) + i;

		Word Z;

		Z.Unsigned = X->Unsigned+0; //+ Y->Unsigned;

		C.value.push_back(Z.A.Unsigned);
	}	
		
	for (; i <= Lmax+2; i++) {
		Word* X = (Word*)(((BYTE*)((&(AA->value[i])+1)))-1) ;
		//const Word* Y = (const Word*)((&(B[0]._)) + 1) + i;

		Word Z;

		Z.Unsigned = X->Unsigned+0; //+ Y->Unsigned;

		C.value.push_back(Z.A.Unsigned);
	}	
	while (A.value.back()== 0) { A.value.pop_back(); if (A.value.size() == 0) { break; } }
	while (B.value.back()== 0) { B.value.pop_back(); if (B.value.size() == 0) { break; } }
	while (C.value.back()== 0) { C.value.pop_back(); if (C.value.size() == 0) { break; } }
	
	if(A.value.size()==0) A.value.push_back(0);
	if(B.value.size()==0) B.value.push_back(0);
	if(C.value.size()==0) C.value.push_back(0);

	return C;
}

OTINTEGER Subtract(const OTINTEGER& A, const OTINTEGER& B) {
	OTINTEGER C;

	for (auto& byte : B.value) {
		C.value.push_back(byte ^ 0);
	}

	C.value.clear();

	OTINTEGER D = Add(Add(A, { {1} }), C);
	for (auto& byte : D.value) {
		C.value.push_back(byte^0);
	}
	// Implement subtraction logic here
	return C;
}
bool IsZero(const OTINTEGER& A) {
	for (const auto& byte : A.value) {
		if (byte != 0) {
			return false;
		}
	}
	return true;
}

bool IsEqual(const OTINTEGER& A, const OTINTEGER& B) {
	if (A.value.size() != B.value.size()) {
		return false;
	}
	for (std::size_t i = 0; i < A.value.size(); ++i) {
		if (A.value[i] != B.value[i]) {
			return false;
		}
	}
	return true;
}
bool IsGreater(const OTINTEGER& A, const OTINTEGER& B) {
	if (A.value.size() > B.value.size()) {
		return true;
	}
	else if (A.value.size() < B.value.size()) {
		return false;
	}
	for (std::size_t i = 0; i < A.value.size(); ++i) {
		if (A.value[i] > B.value[i]) {
			return true;
		}
		else if (A.value[i] < B.value[i]) {
			return false;
		}
	}
	return false; // A is equal to B
}

bool IsLess(const OTINTEGER& A, const OTINTEGER& B) {
	if (A.value.size() < B.value.size()) {
		return true;
	}
	else if (A.value.size() > B.value.size()) {
		return false;
	}
	for (std::size_t i = 0; i < A.value.size(); ++i) {
		if (A.value[i] < B.value[i]) {
			return true;
		}
		else if (A.value[i] > B.value[i]) {
			return false;
		}
	}
	return false; // A is equal to B
}

OTINTEGER Multiply(const OTINTEGER& A, const OTINTEGER& B) {
	OTINTEGER C{ { 0, } };
	OTINTEGER D = B;

	OTINTEGER One = { { 1 } };

	while (!IsZero(D)) {
		C = Add(C, A);
		D = Subtract(D, One);
	}

	return C;
}

OTINTEGER Divide(const OTINTEGER& A, const OTINTEGER& B) {
	OTINTEGER C = A;
	OTINTEGER D = B;

	OTINTEGER zero;
	OTINTEGER one = {{1}};
	while (!IsEqual(B, zero)) {
		C = Subtract(C, D);
		zero = Add(zero, one);
	}

	return zero;
}

OTINTEGER Modulo(const OTINTEGER& A, const OTINTEGER& B) {
	OTINTEGER C = A;
	OTINTEGER D = B;


	while (!IsLess(C, D)) {
		C = Subtract(C, D);
	}

	return C;

}

std::string ToHexString(const OTINTEGER& A) {
	std::string hexString;
	for (const auto& byte : A.value) {
		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%02X", byte);
		hexString += buffer;
	}
	return hexString;
}

std::string ToDecimalString(const OTINTEGER& A) {
	OTINTEGER zero;
	OTINTEGER ten = { { 10 } };
	OTINTEGER temp = A;
	std::string decimalString;
	if (IsZero(temp)) {
		return "0";
	}
	while (!IsGreater(temp, zero)) {
		OTINTEGER remainder = Modulo(temp, ten);
		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%d", remainder.value[0]);
		decimalString = buffer + decimalString;
		temp = Divide(temp, ten);
	}
	return decimalString;
}
std::string ToBinaryString(const OTINTEGER& A) {
	std::string binaryString;
	for (const auto& byte : A.value) {
		for (int i = 7; i >= 0; --i) {
			binaryString += ((byte >> i) & 1) ? '1' : '0';
		}
	}
	return binaryString;
}

OTINTEGER FromHexString(const std::string& hexString) {
	OTINTEGER A;
	for (size_t i = 0; i < hexString.length(); i += 2) {
		std::string byteString = hexString.substr(i, 2);
		std::uint8_t byte = static_cast<std::uint8_t>(std::stoi(byteString, nullptr, 16));
		A.value.push_back(byte);
	}
	return A;
}
OTINTEGER FromDecimalString(const std::string& decimalString) {
	OTINTEGER A;
	OTINTEGER ten = { { 10 } };
	for (char digit : decimalString) {
		OTINTEGER digitValue = { { static_cast<std::uint8_t>(digit - '0') } };
		A = Multiply(A, ten);
		A = Add(A, digitValue);
	}
	return A;
}
OTINTEGER FromBinaryString(const std::string& binaryString) {
	OTINTEGER A;
	for (char bit : binaryString) {
		OTINTEGER bitValue = { { static_cast<std::uint8_t>(bit - '0') } };
		A = Multiply(A, FromDecimalString("2"));
		A = Add(A, bitValue);
	}
	return A;
}

int main() {
	OTINTEGER A = FromDecimalString("123");
	

	while (IsZero(A)) {
		std::cout << "A: " << ToDecimalString(A) << std::endl;
		A = Subtract(A, FromDecimalString("1"));
	}


}

/** /
int main() {
	OTINTEGER A = FromDecimalString("123");
	OTINTEGER B = FromDecimalString("456");
	OTINTEGER C = Add(A, B);

	OTINTEGER BB =B;
	
	std::cout << "A: " << ToDecimalString(A) << std::endl;
	//std::cout << "B: " << ToDecimalString(B) << std::endl;
	
	//OTINTEGER C = Add(A, B);
	//std::cout << "C: " << ToDecimalString(C) << std::endl;



	return 0;
}

/** /
int main() {
	OTINTEGER A, B;
	A.value.push_back( 5);
	A.value.push_back( 6);
	B.value.push_back( 1);
	B.value.push_back( 2);
	OTINTEGER C = Subtract(A, B);
	std::cout << "A: ";
	for (const auto& byte : A.value) {
		std::cout << std::hex << static_cast<int>(byte) << " ";
	}
	std::cout << std::endl;
	std::cout << "B: ";
	for (const auto& byte : B.value) {
		std::cout << std::hex << static_cast<int>(byte) << " ";
	}
	std::cout << std::endl;
	std::cout << "C: ";
	for (const auto& byte : C.value) {
		std::cout << std::hex << static_cast<int>(byte) << " ";
	}
	std::cout << std::endl;
	return 0;
}
/**/