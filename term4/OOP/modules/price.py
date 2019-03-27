from decimal import Decimal

def transform_to_number(price: str) -> Decimal:
    if price[0] == '$':
        return Decimal(price[1:])
    else:
        return Decimal("0")
    
def transform_to_price(price: Decimal) -> str:
    return f"${price}"