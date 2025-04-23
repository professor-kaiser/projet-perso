#ifndef __REFLECTABLE__
#define __REFLECTABLE__

#include <map>
#include <any>
#include <string>
#include <typeindex>
#include <functional>

#define meta_class(CLASS) class CLASS : public reflect::Reflectable
#define extends(...) , __VA_ARGS__

namespace reflect
{
	class Reflectable;

	template <typename T>
	concept ReflectableClass = std::derived_from<T, reflect::Reflectable>;

	class Reflectable
	{
	protected:
		std::map<std::pair<std::type_index, std::string>, std::any> methods;

	public:
		Reflectable() = default;
		virtual ~Reflectable() = default;

		static Reflectable meta;
		/*static Reflectable& meta() {
	        static Reflectable instance;
	        return instance;
	    }*/

		template <typename Ret, ReflectableClass T, typename... Args>
		void reflect(std::string key, Ret (T::*method)(Args...));

		template <typename Ret, ReflectableClass T, typename... Args>
		void reflect(std::string key, std::function<Ret (T&, Args...)> func);

		template <typename F>
		void reflect(std::string key, F&& func) {
		    reflect(key, std::function(func));
		}

		template <typename Ret, ReflectableClass T, typename... Args>
		void reflect(std::string key, std::function<Ret (T*, Args...)> func);

		template <typename Ret, ReflectableClass T, typename... Args>
		void reflect(std::string key, Ret (*method)(Args...));

		template<ReflectableClass T>
		bool exists(std::string key);

		/**
		 *------------------------------------------------------------*
		 *	Used for declared class 								  *
		 *------------------------------------------------------------*
		 * 	@arg clazz  -> T&										  *
		 * 	@arg key    -> std::string								  *
		 * 	@arg params -> Args...									  *
		 *------------------------------------------------------------*/
		template <typename Ret, ReflectableClass T, typename... Args>
		Ret invoke(T& clazz, std::string key, Args... params);
		/*------------------------------------------------------------*/

		/**
		 *------------------------------------------------------------*
		 *	Used for declared class 								  *
		 *------------------------------------------------------------*
		 * 	@arg clazz  -> T*										  *
		 * 	@arg key    -> std::string								  *
		 * 	@arg params -> Args...									  *
		 *------------------------------------------------------------*/
		template <typename Ret, ReflectableClass T, typename... Args>
		Ret invoke(T* clazz, std::string key, Args...);
		/*------------------------------------------------------------*/

		template <typename Ret, ReflectableClass T, typename... Args>
		Ret invoke(std::string key, Args...);

		/**
		 *------------------------------------------------------------*
		 *	Used for static method 								  	  *
		 *------------------------------------------------------------*
		 * 	@arg null   -> std::nullptr_t							  *
		 * 	@arg key    -> std::string								  *
		 * 	@arg params -> Args...									  *
		 *------------------------------------------------------------*/
		template <typename Ret, ReflectableClass T, typename... Args>
		Ret invoke(std::nullptr_t null, std::string key, Args...);
		/*------------------------------------------------------------*/
	};
}

#include "Reflectable.tpp"

#endif