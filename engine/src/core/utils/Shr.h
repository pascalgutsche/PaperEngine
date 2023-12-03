#pragma once

#include <memory>
#include <atomic>


namespace Paper
{
	class ShrCounted
	{
	public:
		virtual ~ShrCounted() = default;

		uint32_t GetShrCount() const { return shrCount.load(); }
	private:
		void IncCount();

		std::atomic<uint32_t>
	};

	template <typename T>
	class Shr
	{
	public:
		Shr()
			: instance(nullptr)
		{
		}

		Shr(std::nullptr_t n)
			: instance(nullptr)
		{
		}

		Shr(T* _instance)
			: instance(_instance)
		{
			static_assert(std::is_base_of<ShrCounted, T>::value, "Class is not SharedCounted");

			IncShr();
		}

		template <typename T2>
		Shr(const Shr<T2>& _instance)
		{
			instance = (T*)_instance.instance;
			IncShr();
		}

		template <typename T2>
		Shr(Shr<T2>&& _instance)
		{
			instance = (T*)_instance.instance;
			_instance.instance = nullptr;
		}

		Shr(const Shr<T>& other)
			: instance(other.instance)
		{
			IncRef();
		}

		~Shr()
		{
			DecRef();
		}

		template<typename T2>
		Shr<T2> As() const
		{
			return Shr<T2>(*this);
		}

		template <typename... TArgs>
		static Shr<T> Create(TArgs&&... args)
		{
			return Shr<T>(new T(std::forward<TArgs>(args)...));
		}

	private: 
		T* instance;
	};

}
