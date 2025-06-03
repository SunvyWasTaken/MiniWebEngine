// Sunset inc.

#pragma once

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };

template <typename ...Ts>
overloads(Ts...) -> overloads<Ts...>;

template <typename Variant, typename ...Callbacks>
decltype(auto) Visit(Variant& var, Callbacks&&... calls)
{
	return std::visit(overloads{calls...}, var);
}

template <typename T, typename T2>
struct VariantIndex
{
};

template <typename T, typename ...Ts>
struct VariantIndex<T, std::variant<T, Ts...>>
{
	static constexpr int value = 0;
};

template <typename T, typename T2, typename ...Ts>
struct VariantIndex<T, std::variant<T2, Ts...>>
{
	static constexpr int value = 1 + VariantIndex<T, std::variant<Ts...>>::value;
};

/************************************************************************/
/* Get Type at Index													*/
/************************************************************************/

template <int i, typename ...Ts>
struct GetVariantAtIndex
{
};

template <typename T, typename ...Ts>
struct GetVariantAtIndex<0, std::variant<T, Ts...>>
{
	using value = T;
};

template <int i, typename T, typename ...Ts>
struct GetVariantAtIndex<i, std::variant<T, Ts...>>
{
	using value = typename GetVariantAtIndex<i - 1, std::variant<Ts...>>::value;
};
