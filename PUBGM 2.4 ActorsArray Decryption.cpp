// Decryption Pattern  00 20 90 E5 E0 10 94 E5 0C 20 92 E5
namespace Decryption
{
	
	struct ActorsEncryption {
		uint32_t Enc_1; //0x0340 
		uint32_t Enc_2; //0x0344 
		uint32_t Enc_3; //0x0348 
		uint32_t Enc_4; //0x034C 
	};
	struct Encryption_Chunk {
		uint32_t val_1, val_2, val_3, val_4;
	};
	uint32_t DecryptActorsArray(uint32_t uLevel, int Actors_Offset, int EncryptedActors_Offset)
	{
		if (uLevel < 0x10000000)
			return 0;

		if (rpm->read<uint32_t>(uLevel + Actors_Offset) > 0)
			return uLevel + Actors_Offset;
		else
		{
			std::cout << "Try Find Next Decryption Address 1" << std::endl;

		}

		if (rpm->read<uint32_t>(uLevel + EncryptedActors_Offset) > 0)
			return uLevel + EncryptedActors_Offset;
		else
		{
			std::cout << "Try Find Next Decryption Address 2" << std::endl;

		}
		auto Encryption = rpm->read<ActorsEncryption>(uLevel + EncryptedActors_Offset + 0x0C);

		if (Encryption.Enc_1 > 0)
		{
			auto Enc = rpm->read<Encryption_Chunk>(Encryption.Enc_1 + 0x80);

			return ((rpm->read<unsigned __int8>(Encryption.Enc_1 + Enc.val_1))
				| ((rpm->read<unsigned __int8>(Encryption.Enc_1 + Enc.val_2)) << 8)
				| ((rpm->read<unsigned __int8>(Encryption.Enc_1 + Enc.val_3)) << 16)
				| ((rpm->read<unsigned __int8>(Encryption.Enc_1 + Enc.val_4)) << 24));
		} //ok
		else if (Encryption.Enc_2 > 0)
		{
			auto Encrypted_Actors = rpm->read<uint32_t>(Encryption.Enc_2);
			if (Encrypted_Actors > 0)
			{
				return (Encrypted_Actors - 0x4000000) & 0xFF000000
					| (Encrypted_Actors + 0xFC0000) & 0xFF0000
					| ((unsigned __int16)Encrypted_Actors - 0x400) & 0xFF00
					| (unsigned __int8)(Encrypted_Actors - 4);
			}
		} //ok
		else if (Encryption.Enc_3 > 0)
		{
			auto Encrypted_Actors = rpm->read<uint32_t>(Encryption.Enc_3);
			if (Encrypted_Actors > 0)
				return __ROR4__(Encrypted_Actors, 0x18);
		}
		else if (Encryption.Enc_4 > 0)
		{
			auto Encrypted_Actors = rpm->read<uint32_t>(Encryption.Enc_4);
			if (Encrypted_Actors > 0)
				return Encrypted_Actors ^ 0xCDCD00;
		}
		else
		{
			std::cout << "Cant Find Decryption Address" << std::endl;
		}
		return 0;
	}
}