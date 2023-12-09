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

		void IncCount() const
		{
			++shrCount;
		}

		void DecCount() const
		{
			--shrCount;
		}

	private:
		mutable std::atomic<uint32_t> shrCount;
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
			IncShr();
		}

		~Shr()
		{
			DecShr();
		}

		template<typename T2>
		Shr<T2> As() const
		{
			return Shr<T2>(*this);
		}

		T* Raw()
		{
			return instance;
		}

		const T* Raw() const
		{
			return instance;
		}

		Shr& operator=(std::nullptr_t)
		{
			DecShr();
			instance = nullptr;
			return *this;
		}

		Shr& operator=(const Shr<T>& other)
		{
			if (this == &other)
				return *this;

			other.IncShr();
			DecShr();

			instance = other.instance;
			return *this;
		}

		template<typename T2>
		Shr& operator=(const Shr<T2>& other)
		{
			other.IncShr();
			DecShr();

			instance = other.instance;
			return *this;
		}

		template<typename T2>
		Shr& operator=(Shr<T2>&& other)
		{
			DecShr();

			instance = other.instance;
			other.instance = nullptr;
			return *this;
		}

		operator bool() { return instance != nullptr; }
		operator bool() const { return instance != nullptr; }

		T* operator->() {return instance; }
		const T* operator->() const {return instance; }

		T& operator*() { return *instance; }
		const T& operator*() const { return *instance; }

		bool operator==(const Shr<T>& other) const
		{
			return instance == other.instance;
		}

		bool operator!=(const Shr<T>& other) const
		{
			return !(*this == other);
		}


		template <typename... TArgs>
		static Shr<T> Create(TArgs&&... args)
		{
			return Shr<T>(new T(std::forward<TArgs>(args)...));
		}

	private:

		void IncShr() const
		{
			if (instance)
				instance->IncCount();
		}

		void DecShr() const
		{
			if (instance)
			{
				instance->DecCount();
				if (instance->GetShrCount() <= 0)
					delete instance;
			}
		}
		template<class T2>
		friend class Shr;
		T* instance = nullptr;
	};

}
