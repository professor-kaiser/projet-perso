#ifndef __REFLECTABLE_TPP__
#define __REFLECTABLE_TPP__

#include <functional>
#include <typeinfo>

namespace reflect
{
	inline Reflectable Reflectable::meta;

	template <typename Ret, ReflectableClass T, typename... Args>
	void
	Reflectable::reflect(std::string key, Ret (T::*method)(Args...))
	{
		std::pair<std::type_index, std::string> pair { std::type_index {typeid(T)}, key };
		if(methods.find(pair) == methods.end())
		{

		}

		methods.insert({pair, std::function<Ret (T&, Args...)>(
			[method](T& clazz, Args... params)
			{
				return (clazz.*method)(std::forward<Args>(params)...);	
			}
		)});
	}

	template <typename Ret, ReflectableClass T, typename... Args>
	void 
	Reflectable::reflect(std::string key, std::function<Ret (T&, Args...)> func)
	{
		std::pair<std::type_index, std::string> pair { std::type_index {typeid(T)}, key };
		if(methods.find(pair) == methods.end())
		{

		}

		methods.insert({ pair, func });
	}

	template <typename Ret, ReflectableClass T, typename... Args>
	void 
	Reflectable::reflect(std::string key, std::function<Ret (T*, Args...)> func)
	{
		std::pair<std::type_index, std::string> pair { std::type_index {typeid(T)}, key };
		if(methods.find(pair) == methods.end())
		{

		}

		methods.insert({ pair, func });
	}

	template <typename Ret, ReflectableClass T, typename... Args>
	void 
	Reflectable::reflect(std::string key, Ret (*method)(Args...))
	{
		std::pair<std::type_index, std::string> pair { std::type_index {typeid(T)}, key };
		if(methods.find(pair) == methods.end())
		{

		}

		methods.insert({pair, std::function<Ret (std::nullptr_t, Args...)>(
			[method](std::nullptr_t null, Args... params)
			{
				return (*method)(std::forward<Args>(params)...);	
			}
		)});
	}

	template<ReflectableClass T>
	bool 
	Reflectable::exists(std::string key)
	{
		std::pair<std::type_index, std::string> pair { std::type_index {typeid(T)}, key };
		if(methods.find(pair) == methods.end())
		{
			
			return false;
		}

		return true;
	}

	template <typename Ret, ReflectableClass T, typename... Args>
	Ret
	Reflectable::invoke(T& clazz, std::string key, Args... args)
	{
		std::pair<std::type_index, std::string> pair { std::type_index(typeid(T)), key };
		auto method { std::any_cast<std::function<Ret (T&, Args...)>>(methods.at(pair)) };

		return method(clazz, std::forward<Args>(args)...);
	}

	template <typename Ret, ReflectableClass T, typename... Args>
	Ret
	Reflectable::invoke(T* clazz, std::string key, Args... args)
	{
		std::pair<std::type_index, std::string> pair { std::type_index(typeid(T)), key };
		auto method { std::any_cast<std::function<Ret (T*, Args...)>>(methods.at(pair)) };

		return method(clazz, std::forward<Args>(args)...);
	}

	template <typename Ret, ReflectableClass T, typename... Args>
	Ret 
	Reflectable::invoke(std::string key, Args... args)
	{
		std::pair<std::type_index, std::string> pair { std::type_index(typeid(T)), key };
		auto method { std::any_cast<std::function<Ret (T&, Args...)>>(methods.at(pair)) };

		return method(*dynamic_cast<T*>(this), std::forward<Args>(args)...);
	}

	template <typename Ret, ReflectableClass T, typename... Args>
	Ret 
	Reflectable::invoke(std::nullptr_t null, std::string key, Args... args)
	{
		std::pair<std::type_index, std::string> pair { std::type_index(typeid(T)), key };
		auto method { std::any_cast<std::function<Ret (std::nullptr_t, Args...)>>(methods.at(pair)) };

		return method(null, std::forward<Args>(args)...);
	}
}

#endif