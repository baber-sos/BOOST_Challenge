#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <array>
#include <initializer_list>
#include <utility>
#include <iostream>
#include <string.h>


namespace bro{
	template <size_t i> struct at {
		
		template<class key, class data, size_t n> 
		inline constexpr data &operator()(std::pair<size_t, std::pair<const key, data>> (arr)[n], const key& ind) const {
			return ind == arr[i].second.first ? arr[i].second.second : at<i-1>()(arr, ind);
		}

		template<class key, class data, size_t n> 
		inline constexpr const data &operator()(const std::pair<size_t, std::pair<const key, data>> (&arr)[n], const key& ind) const{
			return ind == arr[i].second.first ? arr[i].second.second : at<i-1>()(arr, ind);
		}
	};
	
	template<> struct at<0>{
		template <class data> 
		inline constexpr data &dummy(const data& x){
			throw std::out_of_range("key not in the map");
		}

		template <class data> 
		inline constexpr const data &dummy(const data& x) const{
			throw std::out_of_range("key not in the map");
		}

		template<class key, class data, size_t n> 
		inline constexpr data &operator()(std::pair<size_t, std::pair<const key, data>> (arr)[n], const key& ind) const {
			return arr[0].second.first == ind ? arr[0].second.second :
			dummy(arr[0].second.second);
		// if (arr[0].second.first == ind)
		// return arr[0].second.second;
		// throw std::out_of_range("key not in map");
		}
		template<class key, class data, size_t n> 
		inline constexpr const data &operator()(const std::pair<size_t, std::pair<const key, data>> (&arr)[n], const key& ind) const {
			return arr[0].second.first == ind ? arr[0].second.second :
			dummy(arr[0].second.second);
		// if (arr[0].second.first == ind)
		// return arr[0].second.second;
		// throw std::out_of_range("key not in map");
		}
	};
}
// template<template<class key, class data> class... ele> constexpr data &actualCheck(ele...
//arr){
// // return;
// }
// template<class key, class data, size_t n, size_t... I> constexpr data
//&findInMap(std::pair<size_t, std::pair<key, data>> (&arr)[n], std::index_sequence<I...>){
// return actualCheck(arr[I]...);
// }
// template<class key, class data, size_t n> constexpr data &lookup(std::pair<size_t,
//std::pair<key, data>> (&arr)[n]){
// return findInMap(arr, std::make_index_sequence<n>());
// }
template<class key, class data, size_t no>
struct static_map{
	using key_type = key;
	using mapped_type = data;
	using value_type = std::pair<const key_type, mapped_type>;
	
	private:
	using _value_type = std::pair<size_t, value_type>;
	_value_type _values[no];
	
	static constexpr _value_type _new_value_type(const std::pair<key, data> &v)
	{
		return std::make_pair(0, std::make_pair(v.first, v.second));
	}
	public:
	template<class... U> constexpr static_map(U &&...il) : _values{ _new_value_type(il)... } {}
	constexpr data& operator[](const key &ind) const{
		return const_cast<data &>(bro::at<no-1>()(_values, ind));
	}
	constexpr data& at(const key &ind) const{
		return const_cast<data &>(bro::at<no-1>()(_values, ind));
	}
};


constexpr std::pair<const size_t, const char*> map[] = {
	{5, "apple"},
	{8, "pear"}
};

template<class key, class data, size_t n, size_t... I> 
constexpr static_map<key, data, n> make_map_from_array(const std::pair<const key, data> (&arr)[n], std::index_sequence<I...>){
	return static_map<key, data, n>(arr[I]...);
}

template<class key, class data, size_t n> 
constexpr static_map<key, data, n> make_map(const std::pair<const key, data> (&arr)[n]){
	return make_map_from_array(arr, std::make_index_sequence<n>());
}
int main(){
	constexpr auto cmap = make_map(map);
	if (!cmap[5]) abort();
	const char* f = cmap[5];
	// std::cout << f << std::endl;
	return 0;
}